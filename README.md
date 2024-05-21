# Mystique Self-Injection

[![](https://img.youtube.com/vi/OpNISD9Ge_c/0.jpg)](https://www.youtube.com/watch?v=OpNISD9Ge_c "Mystique Self-Injection")


------------

**Mystique Self-Injection** represents an advancement over the Mockingjay Self-Injection method by eliminating the dependency on a trusted DLL with RWX sections. Instead, it leverages the NtProtectVirtualMemory function to enable self-modification in the .text section, offering a more streamlined approach. By dynamically inserting system call stubs and manipulating permissions, the technique facilitates the automated modification of the .text section, complicating static analysis and challenging traditional security solutions. While not circumventing kernel callbacks or application behavior collection methods, it underscores the need for continuous innovation in threat detection and response capabilities. 

This Proof of Concept (PoC) represents a TCP server that listens for a payload on port 20248. Upon receiving it, the server overwrites its own .text section and executes the payload, eliminating the need for RWX memory allocation using traditional methods.


------------

###Links

[Self-Modifying Code Sections with WriteProcessMemory for EDR Evasion](https://revflash.medium.com/its-morphin-time-self-modifying-code-sections-with-writeprocessmemory-for-edr-evasion-9bf9e7b7dced)