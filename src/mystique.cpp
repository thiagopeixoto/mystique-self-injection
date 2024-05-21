#include "mystique.h"

const SIZE_T PLACEHOLDER_SIZE = 4096;

bool Mystique::generate_syscall_stub() 
{
    auto sys_call_info = get_syscall_number_and_test_address(this->NtProtectVirtualMemory);

    if (std::get<0>(sys_call_info) == INVALID_SYSCALL_NUMBER || !std::get<1>(sys_call_info)) {
        printf("[x] Failed to get system call data\n");
        return false;
    } else {
        printf("[*] NtProtectVirtualMemory {\n\tSyscall number: 0x%X, \n\tTest instruction address: 0x%p\n}\n", 
            std::get<0>(sys_call_info), 
            (PVOID)std::get<1>(sys_call_info)
        );
    }

    SIZE_T placeholder_size = 32;
    ULONG old_protection = 0;
    PVOID placeholder_address = (PVOID)syscall_placeholder;
    PVOID placeholder_page_start_address = placeholder_address;
    
    auto status = this->NtProtectVirtualMemory(
        ::GetCurrentProcess(),
        &placeholder_page_start_address,
        &placeholder_size,
        PAGE_EXECUTE_READWRITE,
        &old_protection
    );
    if (status != 0) {
        printf("[x] NtProtectVirtualMemory failed with status 0x%X\n", status);
        return false;
    }

    create_syscall_stub(placeholder_address, std::get<0>(sys_call_info), std::get<1>(sys_call_info));

    status = this->NtProtectVirtualMemory(
        ::GetCurrentProcess(),
        &placeholder_page_start_address,
        &placeholder_size,
        PAGE_EXECUTE_READ,
        &old_protection
    );
    if (status != 0) {
        printf("[x] NtProtectVirtualMemory failed with status 0x%X\n", status);
        return false;
    }

    return true;
}

bool Mystique::write_data(PVOID data, SIZE_T size) 
{
    if (size > PLACEHOLDER_SIZE) {
        printf("[x] Data size is larger than the placeholder size\n");
        return false;
    } else {
        SIZE_T placeholder_size = 4096;
        ULONG old_protection = 0;
        PVOID placeholder_address = (PVOID)function_placeholder;
        PVOID placeholder_page_start_address = placeholder_address;

        auto syscall_stub_code = reinterpret_cast<NTPROTECTVIRTUALMEMORY>(&syscall_placeholder);
            
        auto status = syscall_stub_code(
            ::GetCurrentProcess(),
            &placeholder_page_start_address,
            &placeholder_size,
            PAGE_EXECUTE_READWRITE,
            &old_protection
        );
        if (status != 0) {
            printf("[x] NtProtectVirtualMemory failed with status 0x%X\n", status);
            return false;
        }

        memcpy(placeholder_address, data, size);

        status = syscall_stub_code(
            ::GetCurrentProcess(),
            &placeholder_page_start_address,
            &placeholder_size,
            PAGE_EXECUTE_READ,
            &old_protection
        );
        if (status != 0) {
            printf("[x] NtProtectVirtualMemory failed with status 0x%X\n", status);
            return false;
        }

        return true;
    }
}

void Mystique::execute() 
{
    function_placeholder();
}