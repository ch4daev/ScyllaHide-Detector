.CODE
get_peb PROC
mov rax, qword ptr GS:[60h]
ret
get_peb ENDP
END