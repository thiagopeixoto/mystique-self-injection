#include "syscall.h"

constexpr int MAX_SYSCALL_SUBS = 5;
constexpr int SYSCALL_STUB_SIZE = 32;

static uint32_t get_syscall_number(PVOID address);
static uint32_t recursive_syscall_number_search_loweraddresses(PVOID address, uint32_t steps);
static uint32_t recursive_syscall_number_search_higheraddresses(PVOID address, uint32_t steps);

static uint32_t syscall_number_from_stub(PVOID address)
{
    uint32_t syscall_number = INVALID_SYSCALL_NUMBER;
    BYTE syscall_stub[SYSCALL_STUB_SIZE];

    memcpy(&syscall_stub, address, SYSCALL_STUB_SIZE);

    if (syscall_stub[3] != 0xB8) {
        printf("[x] Failed to get the system call number\n");
    } else {
        syscall_number = *(uint32_t *)&syscall_stub[4];
    }

    return syscall_number;
}

static uint64_t syscall_test_address_from_stub(PVOID address)
{
    uint64_t test_address = 0;

    test_address = (uint64_t)address + 0x8;

    return test_address;
}

std::tuple<uint32_t, uint64_t> get_syscall_number_and_test_address(PVOID address)
{
    uint32_t syscall_number     = get_syscall_number(address);
    uint64_t test_instr_address = syscall_test_address_from_stub(address);

    return {syscall_number, test_instr_address};
}

static uint32_t get_syscall_number(PVOID address)
{   
    uint32_t syscall_number = recursive_syscall_number_search_loweraddresses(address, 0);
    
    if (syscall_number != INVALID_SYSCALL_NUMBER) {
        return syscall_number;
    } else {
        syscall_number = recursive_syscall_number_search_higheraddresses(address, 0);
        if (syscall_number != INVALID_SYSCALL_NUMBER) {
            return syscall_number;
        } else {
            return INVALID_SYSCALL_NUMBER;
        }
    }
}

static uint32_t recursive_syscall_number_search_loweraddresses(PVOID address, uint32_t steps)
{
    if (steps >= MAX_SYSCALL_SUBS) {
        return INVALID_SYSCALL_NUMBER;
    } else {
        uint32_t syscall_number = syscall_number_from_stub(address);

        if (syscall_number == INVALID_SYSCALL_NUMBER) {
            return recursive_syscall_number_search_loweraddresses(((PBYTE)address - SYSCALL_STUB_SIZE), steps + 1);
        } else {
            return syscall_number + steps;
        }
    }
}

static uint32_t recursive_syscall_number_search_higheraddresses(PVOID address, uint32_t steps)
{
    if (steps >= MAX_SYSCALL_SUBS) {
        return INVALID_SYSCALL_NUMBER;
    } else {
        uint32_t syscall_number = syscall_number_from_stub(address);
        
        if (syscall_number == INVALID_SYSCALL_NUMBER) {
            return recursive_syscall_number_search_higheraddresses(((PBYTE)address + SYSCALL_STUB_SIZE), steps + 1);
        } else {
            return syscall_number - steps;
        }
    }
}

void create_syscall_stub(PVOID placeholder, uint32_t syscall_number, uint64_t test_instr_address)
{
    
    BYTE stub[] = {
        0x49, 0x89, 0xca,                           // mov r10, rcx
        0xb8, 0x00, 0x00, 0x00, 0x00,               // mov eax, 0x0
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,         // jmp qword ptr [rip]
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    ULONG stub_size = sizeof(stub);
    SIZE_T bytes_written = 0;

    memcpy((uint32_t *)&stub[4], &syscall_number, sizeof(uint32_t));
    memcpy((PVOID *)&stub[14], &test_instr_address, sizeof(uint64_t));

    memcpy(placeholder, &stub, sizeof(stub));
}