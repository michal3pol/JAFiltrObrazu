public MyProc1
.code
MyProc1 proc x: DWORD, y: DWORD
 xor eax,eax ; EAX = 0
 mov eax,x ; Param1 eax = x
 mov ecx,y ; Param2 ecx = y
 ror ecx,1 ; shift ecx right by 1
 shld eax,ecx,2 ; set flags registry
 jnc ET1
 mul y
 ret ; return z in EAX register
 ET1: mul x
 neg y
 ret ; return z in EAX register
MyProc1 endp
end ; End of ASM file
