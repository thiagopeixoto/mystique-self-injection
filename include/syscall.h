#ifndef SYSCALL_H
#define SYSCALL_H

#include <tuple>
#include <cstdio>
#include <Windows.h>

constexpr uint32_t INVALID_SYSCALL_NUMBER = ~0;

std::tuple<uint32_t, uint64_t> get_syscall_number_and_test_address(PVOID address);
void create_syscall_stub(PVOID placeholder, uint32_t syscall_number, uint64_t test_instr_address);

#endif /* SYSCALL_H */
