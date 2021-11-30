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
mov r14, rcx ;colors (input array)
mov r15, rcx ;colors_filtered (output array)
;variables
;r12d - paddingamount
mov r12d, r10d ;r12 = width
imul r12d, 3
xor edx, edx ;cleaning 
mov r13d, 4 ;divisor
mov eax, r12d
div r13d
mov r12d, edx ;remainder in r12d (width*3)%4
mov edx, 4 
sub edx, r12d ; 4 - (width*3)%4
mov edx, r12d; r12d do edx
div r13d
mov r12d,edx ;paddingamount in r12d (4 - (width*3)%4)%4

;r13d = row
mov r13d, r10d
imul r13d, 3 
add r13d, r12d

;if(start_height == 0)
cmp r8d, 0 
je first_row
jmp short check_last

first_row:
	pinsrb xmm0, BYTE PTR[rcx],0 ;byte to xmm0 
	pinsrb xmm0, BYTE PTR[rcx+1],1
	pinsrb xmm0, BYTE PTR[rcx+2],2


	;todo loops

add r8d, 1 ;actualrow++

check_last:  ;todo
	cmp r9d, r11d ;if (stop_height == height)
	je last_row 
	jmp another_rows 

another_rows:
;compute index - result in rsi
	xor rsi, rsi
	mov rsi, r8 ;rsi= actualrow
	imul rsi, r13 ;rsi*row
loop_rows:
	;first pixel
	add rsi, r13 ;+row
	pinsrb xmm0, BYTE PTR[r14 + rsi],0 ;bgr to xmm0 colors[i + row + 3];
	pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
	pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
	sub rsi, r13 ;-row
	pextrb BYTE PTR[r15 + rsi], xmm0,0
	pextrb BYTE PTR[r15 + rsi +1], xmm0,1
	pextrb BYTE PTR[r15 + rsi +2], xmm0,2
	;xor al, al
	;mov al,  BYTE PTR[r14+ rsi + r13 + 3]    ; B - colors_filtered[i] = colors[i] 
	;mov BYTE PTR[r15+ rsi + r13], al
	;add rsi, 1
	;mov al,  BYTE PTR[r14+ rsi + r13 + 3]    ; G - colors_filtered[i] = colors[i] 
	;mov BYTE PTR[r15+ rsi+ r13], al	  
	;add rsi, 1
	;mov al,  BYTE PTR[r14+ rsi + r13 +3]    ; R - colors_filtered[i] = colors[i] 
	;mov BYTE PTR[r15 + rsi+ r13 ], al	
	;add rsi, 1
	
	;sprawdzic ile razy moge dodawac po 16 - zmienna w rdi i reszta pozniej 
		add rsi, r13 ;+row
		pinsrb xmm0, BYTE PTR[r14 + rsi+3],0 ;byte to xmm0 - r14-adress rsi-index +3
		pinsrb xmm0, BYTE PTR[r14 + rsi+4],1 ;colors[i+row+3]
		pinsrb xmm0, BYTE PTR[r14 + rsi+5],2
		pinsrb xmm0, BYTE PTR[r14 + rsi+6],3
		pinsrb xmm0, BYTE PTR[r14 + rsi+7],4
		pinsrb xmm0, BYTE PTR[r14 + rsi+8],5
		pinsrb xmm0, BYTE PTR[r14 + rsi+9],6
		pinsrb xmm0, BYTE PTR[r14 + rsi+10],7
		pinsrb xmm0, BYTE PTR[r14 + rsi+11],8
		pinsrb xmm0, BYTE PTR[r14 + rsi+12],9
		pinsrb xmm0, BYTE PTR[r14 + rsi+13],10
		pinsrb xmm0, BYTE PTR[r14 + rsi+14],11
		pinsrb xmm0, BYTE PTR[r14 + rsi+15],12
		pinsrb xmm0, BYTE PTR[r14 + rsi+16],13
		pinsrb xmm0, BYTE PTR[r14 + rsi+17],14
		pinsrb xmm0, BYTE PTR[r14 + rsi+18],15
		sub rsi, r13 ;-row
		sub rsi, r13 ;-row
		pinsrb xmm1, BYTE PTR[r14 + rsi-3],0 ;byte to xmm0 - r14-adress rsi-index +3
		pinsrb xmm1, BYTE PTR[r14 + rsi-2],1 ;colors[i-row-3]
		pinsrb xmm1, BYTE PTR[r14 + rsi-1],2
		pinsrb xmm1, BYTE PTR[r14 + rsi],3
		pinsrb xmm1, BYTE PTR[r14 + rsi+1],4
		pinsrb xmm1, BYTE PTR[r14 + rsi+2],5
		pinsrb xmm1, BYTE PTR[r14 + rsi+3],6
		pinsrb xmm1, BYTE PTR[r14 + rsi+4],7
		pinsrb xmm1, BYTE PTR[r14 + rsi+5],8
		pinsrb xmm1, BYTE PTR[r14 + rsi+6],9
		pinsrb xmm1, BYTE PTR[r14 + rsi+7],10
		pinsrb xmm1, BYTE PTR[r14 + rsi+8],11
		pinsrb xmm1, BYTE PTR[r14 + rsi+9],12
		pinsrb xmm1, BYTE PTR[r14 + rsi+10],13
		pinsrb xmm1, BYTE PTR[r14 + rsi+11],14
		pinsrb xmm1, BYTE PTR[r14 + rsi+12],15
		;!!!!!!!!!!!!odjac xmm0-xmm1
		add rsi, r13 ;+row
		pextrb BYTE PTR[r15 + rsi], xmm0,0
		pextrb BYTE PTR[r15 + rsi + 1], xmm0,1
		pextrb BYTE PTR[r15 + rsi + 2], xmm0,2
		pextrb BYTE PTR[r15 + rsi + 3], xmm0,3
		pextrb BYTE PTR[r15 + rsi + 4], xmm0,4
		pextrb BYTE PTR[r15 + rsi + 5], xmm0,5
		pextrb BYTE PTR[r15 + rsi + 6], xmm0,6
		pextrb BYTE PTR[r15 + rsi + 7], xmm0,7
		pextrb BYTE PTR[r15 + rsi + 8], xmm0,8
		pextrb BYTE PTR[r15 + rsi + 9], xmm0,9
		pextrb BYTE PTR[r15 + rsi + 10], xmm0,10
		pextrb BYTE PTR[r15 + rsi + 11], xmm0,11
		pextrb BYTE PTR[r15 + rsi + 12], xmm0,12
		pextrb BYTE PTR[r15 + rsi + 13], xmm0,13
		pextrb BYTE PTR[r15 + rsi + 14], xmm0,14
		pextrb BYTE PTR[r15 + rsi + 15], xmm0,15
		add rsi, 16
		;sprawdzic zmienna co zrobie

	;last pixel
	add rsi, r13 ;+row
	pinsrb xmm0, BYTE PTR[r14 + rsi],0 ;bgr to xmm0 colors[i + row + 3];
	pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
	pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
	sub rsi, r13 ;-row
	pextrb BYTE PTR[r15 + rsi], xmm0,0
	pextrb BYTE PTR[r15 + rsi +1], xmm0,1
	pextrb BYTE PTR[r15 + rsi +2], xmm0,2

cmp r8d, r9d ;if actual_row == stop_height
je proc_end
add r8d, 1 ;actualrow++
add rsi, r13 ;increase index + row  -rsi
jmp loop_rows

last_row:
;todo
sub r9d, 1; --stopheight
jmp another_rows


proc_end:	;remember to recover nonvolatile registers
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