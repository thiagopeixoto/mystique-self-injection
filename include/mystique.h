#ifndef MYSTIQUE_H
#define MYSTIQUE_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdlib>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "syscall.h"

typedef NTSTATUS (NTAPI *NTPROTECTVIRTUALMEMORY)(
    HANDLE ProcessHandle,
    PVOID *UnsafeBaseAddress,
    SIZE_T *UnsafeNumberOfBytesToProtect,
    ULONG NewAccessProtection,
    PULONG UnsafeOldAccessProtection
);

extern "C" void syscall_placeholder(void);
extern "C" void function_placeholder(void);

class Mystique {
public: 
    Mystique() {
        this->NtProtectVirtualMemory = (NTPROTECTVIRTUALMEMORY)::GetProcAddress(
            ::GetModuleHandleW(L"ntdll.dll"), 
            "NtProtectVirtualMemory"
        );
    }
    bool generate_syscall_stub();
    bool write_data(PVOID data, SIZE_T size);
    void execute();
private:
    NTPROTECTVIRTUALMEMORY NtProtectVirtualMemory = nullptr;
    PVOID data = nullptr;
    SIZE_T size = 0;
};

#endif /* MYSTIQUE_H */
