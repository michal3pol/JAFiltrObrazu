;J�zyki Asemblerowe
;Filtrowanie obrazu - filtr uwypuklaj�cy
;Ka�da ze sk�adowych obrazu RGB przekszta�cana jest przez mask�, tak aby wykry� kraw�dzie w obrazie. Efekt powinien by� por�wnywalny do p�askorze�by
;11.01.2022 r. Rok akademicki: 3, semestr: 5, grupa: V
;Autor: Micha� Foks
;wersja: 1.0

;args:
;rcx -1st arg (colors), 
;rdx -2nd arg (colors_filtered) 
;r8d -3rd arg (start_height)
;r9d -4th arg (stop_height)
;r10d -5th arg (width)
;r11d -6th arg (height)

;Procedura embossingFilter - filtruje obraz nak�adaj�c mask� na ka�d� ze sk�adowych obrazu
;Parametry: colors - wska�nik na tablice z warto�ciami RGB obrazu wej�ciowego 
;			colors_filtered - wska�nik na tablic� z warto�ciami RGB obrazu przefiltrowanego
;			start_height - numer wiersza od kt�rego przetwarzanie ma si� rozpocz��
;			stop_height -  numer wiersza na kt�rym przetwarzanie ma si� zako�czy�
;			width - szeroko�� obrazu
;			height - wysoko�� obrazu
;Rejestry ulegaj�ce zmianom: rbp, rbx, rsi, rdi, rcx, rsp, rdx, rax, r8-r15, xmm0, xmm1
;Flagi ulegaj�ce zmianie: ZR, PE
public embossingFilter
.code
embossingFilter proc
mov r10d,DWORD PTR[rsp+40] ;5th arg r10d=width
mov r11d,DWORD PTR[rsp+48] ;6th arg r11d=height
;pushing on stack nonvolatile registers
push rbp
push rbx
push rsi
push rdi
push r12
push r13
push r14
push r15
mov r14, rcx ;colors (input array)
mov r15, rdx ;colors_filtered (output array)
;variables
;r12d - paddingamount
mov r12d, r10d	;r12 = width
imul r12d, 3
xor edx, edx	;cleaning 
mov r13d, 4		;divisor
mov eax, r12d
div r13d
mov r12d, edx	;remainder in r12d (width*3)%4
mov edx, 4 
sub edx, r12d	;4 - (width*3)%4
mov edx, r12d	;r12d -> edx
div r13d
mov r12d,edx	;paddingamount in r12d (4 - (width*3)%4)%4

;r13d = row
mov r13d, r10d
imul r13d, 3 
add r13d, r12d

;if(start_height == 0)
cmp r8d, 0 
je first_row
jmp check_last

first_row:
	xor rsi, rsi	;rsi - actual index of r,g,b
	xor edx, edx
	mov edx, r10d	;r10d - width
	imul edx, 3 
	sub edx, 3		;(width*3)-3 - index of last pixel in loop
	loop_pix_1st_row:
		add rsi, r13	;+row - colors[i+row+3]
		pinsrb xmm0, BYTE PTR[r14 + rsi +3],0 ;byte to xmm0 
		pinsrb xmm0, BYTE PTR[r14 + rsi +4],1
		pinsrb xmm0, BYTE PTR[r14 + rsi +5],2
		sub rsi, r13	;-row - colors_filtered[i+3]
		pextrb BYTE PTR[r15 + rsi], xmm0,0
		pextrb BYTE PTR[r15 + rsi +1], xmm0,1
		pextrb BYTE PTR[r15 + rsi +2], xmm0,2
		add rsi, 3 
		cmp rsi, rdx	;if(index == (width*3)-3)
		je last_pix_1st_row
		jmp loop_pix_1st_row
	last_pix_1st_row:
		pinsrb xmm0, BYTE PTR[r14 + rsi],0 ;colorsfiltered[i] = colors[i]
		pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
		pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
		pextrb BYTE PTR[r15 + rsi], xmm0,0
		pextrb BYTE PTR[r15 + rsi +1], xmm0,1
		pextrb BYTE PTR[r15 + rsi +2], xmm0,2
		add r8d, 1		;actualrow++

check_last:  
	cmp r9d, r11d	;if (stop_height == height)
	je last_row 
	jmp another_rows 

another_rows:
;compute index - result in rsi
	xor rsi, rsi
	mov rsi, r8		;rsi= actualrow
	imul rsi, r13	;rsi*row

	;(width*3)%16 -> how many loops for 16 rgb
	xor edx, edx
	mov eax, r10d
	imul eax, 3
	sub eax, 3	;-3 cuz last pixel -> another mask 
	mov ecx, 16
	div ecx		;in eax how many times loop, and rest of gbr in edx
	cmp eax, 0  ;less than 3/2 pixel
	je pix_rest

loop_rows:
	xor edx, edx
	mov eax, r10d
	imul eax, 3
	sub eax, 3	;-3 cuz last pixel -> another mask 
	mov ecx, 16
	div ecx		;in eax how many times loop, and rest of gbr in edx
	cmp eax, 0  ;less than 3,2 pixel
	je pix_rest

	;first pixel
	add rsi, r13 ;+row
	pinsrb xmm0, BYTE PTR[r14 + rsi],0 ;bgr to xmm0 colors[i + row + 3];
	pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
	pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
	sub rsi, r13 ;-row
	pextrb BYTE PTR[r15 + rsi], xmm0,0
	pextrb BYTE PTR[r15 + rsi +1], xmm0,1
	pextrb BYTE PTR[r15 + rsi +2], xmm0,2

	loop_pix_xmm:
		add rsi, r13 ;+row [i+row]
		movups xmm0, [r14 + rsi+3]	;move 16 bytes from memory to xmm
		sub rsi, r13 ;-row [i]
		sub rsi, r13 ;-row [i-row]
		movups xmm1, [r14 + rsi-3]	;move 16 bytes from memory to xmm
		psubb xmm0, xmm1			;substract xmm0-xmm1
		add rsi, r13 ;+row [i]
		movups [r15 + rsi], xmm0	;move 16 bytes from xmm to memory
		add rsi, 16
		sub eax, 1
		cmp eax, 0 ; if there are no more 16rgb
		je pix_rest
		jmp loop_pix_xmm

		pix_rest:	;in edx rest of rgb to compete
		cmp edx, 0 ;no rgb to compete 
		mov rbx, 0
		je last_pixel_row
			loop_pix_rest:
			add rsi, r13 ;+row [i+row] 
			mov bl, BYTE PTR[r14 + rsi + 3]
			sub rsi, r13 ;-row [i]
			sub rsi, r13 ;-row [i-row]
			sub bl,	BYTE PTR[r14 + rsi - 3]
			add rsi, r13 ;+row [i] 
			mov BYTE PTR[r15 + rsi], bl	;save in output table
			add rsi, 1
			sub edx, 1
			cmp edx, 0
			je last_pixel_row
			jmp loop_pix_rest

	last_pixel_row:
	add rsi, r13 ;+row
	pinsrb xmm0, BYTE PTR[r14 + rsi],0 ;bgr to xmm0 colors[i + row + 3];
	pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
	pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
	sub rsi, r13 ;-row
	pextrb BYTE PTR[r15 + rsi], xmm0,0
	pextrb BYTE PTR[r15 + rsi +1], xmm0,1
	pextrb BYTE PTR[r15 + rsi +2], xmm0,2

	add r8d, 1 ;actualrow++
	cmp r8d, r9d ;if actual_row == stop_height
	je proc_end
	add rsi, r12 ;increase index + padding ->rsi
	jmp loop_rows

last_row:
	xor rsi, rsi	;rsi - actual index of r,g,b
	mov rsi, r11 
	sub rsi, 1 
	imul rsi, r13	;row * (height - 1)
	xor edx, edx	;it will be last index 
	mov edx, r10d	;width
	imul edx, 3		;width*3
	add edx, esi	;+index of 1st pixel in last row =  row * (height - 1) + (width * 3)
	;1st pixel last row
		pinsrb xmm0, BYTE PTR[r14 + rsi],0	;colorsfiltered[i] = colors[i]
		pinsrb xmm0, BYTE PTR[r14 + rsi + 1],1
		pinsrb xmm0, BYTE PTR[r14 + rsi + 2],2
		pextrb BYTE PTR[r15 + rsi], xmm0,0
		pextrb BYTE PTR[r15 + rsi +1], xmm0,1
		pextrb BYTE PTR[r15 + rsi +2], xmm0,2
	loop_pix_last_row:
		sub rsi, r13 ;-row - colors[i-row-3]
		pinsrb xmm0, BYTE PTR[r14 + rsi -3],0 ;byte to xmm0 
		pinsrb xmm0, BYTE PTR[r14 + rsi -2],1
		pinsrb xmm0, BYTE PTR[r14 + rsi -1],2
		add rsi, r13 ;+row - colors_filtered[i]
		pextrb BYTE PTR[r15 + rsi], xmm0,0
		pextrb BYTE PTR[r15 + rsi +1], xmm0,1
		pextrb BYTE PTR[r15 + rsi +2], xmm0,2
		add rsi, 3 
		cmp rsi, rdx ; if(index == row * (height - 1) + width)
		je back_to_another_rows
		jmp loop_pix_last_row
	back_to_another_rows:
	sub r9d, 1	;--stopheight
	jmp another_rows

proc_end:	;remember to recover nonvolatile registers
pop r15
pop r14
pop r13
pop r12
pop rdi
pop rsi
pop rbx
pop rbp

ret
embossingFilter endp
end