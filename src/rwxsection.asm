.CODE
PUBLIC function_placeholder
PUBLIC syscall_placeholder

function_placeholder PROC
	DB 4096 DUP(0)
function_placeholder ENDP

syscall_placeholder PROC
	DB 32 DUP(0)
syscall_placeholder ENDP

END