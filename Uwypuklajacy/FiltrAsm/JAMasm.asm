;rcx -1st arg (colors), 
;rdx -2nd arg (colors_filtered) 
;r8d -3rd arg (start_height)
;r9d -4th arg (stop_height)
;r10d -5th arg (width)
;r11d -6th arg (height)
public embossingFilter
.code
embossingFilter proc
;pushing on stack nonvolatile registers
push rbp
push rbx
push rsi
push rdi
push r12
push r13
push r14
push r15
mov r10d,DWORD PTR[rsp+40] ;5th arg r10d=width
mov r11d,DWORD PTR[rsp+48] ;6th arg r11d=height
mov r14, rcx 
;variables
;r12d - paddingamount
mov r12d, r10d ;r12 = width
imul r12d, 3
xor edx, edx ;cleaning 
mov r13d, 4 ;divisor
mov eax, r12d;gdzie wrzucic r12d? do edx?
div r13d
mov r12d, edx ;remainder in r12d (width*3)%4
mov edx, 4 
sub edx, r12d ; 4 - (width*3)%4
mov edx, r12d;gdzie wrzucic r12d? do edx?
div r13d
mov r12d,edx ;paddingamount in r12d (4 - (width*3)%4)%4

;r13d = row
mov r13d, r10d
imul r13d, 3 
add r13d, r12d

;if(start_height == 0)
cmp r8d, 0 
je first_row
jmp short another_rows


first_row:
pinsrb xmm0, BYTE PTR[rcx],0 ;byte to xmm0
pinsrb xmm0, BYTE PTR[rcx+1],1
pinsrb xmm0, BYTE PTR[rcx+2],2
pinsrb xmm0, BYTE PTR[rcx+3],3
pinsrb xmm0, BYTE PTR[rcx+4],4
pinsrb xmm0, BYTE PTR[rcx+5],5
pinsrb xmm0, BYTE PTR[rcx+6],6
pinsrb xmm0, BYTE PTR[rcx+7],7


	;todo loops

add r8d, 1 ;actualrow++

another_rows:
	;todo

;remember to recover nonvolatile registers
pop rdi
pop rsi
pop rbx
pop rbp
pop r12
pop r13
pop r14
pop r15

ret
embossingFilter endp
end