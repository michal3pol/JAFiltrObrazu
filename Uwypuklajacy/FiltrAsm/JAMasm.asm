;ecx -1st arg (colors), 
;edx -2nd arg (colors_filtered)
;r8d -3rd arg (start_height)
;r9d -4th arg (stop_height)
;ebx -5th arg (width)
;eax -6th arg (height)
public embossingFilter
.code
embossingFilter proc
mov ebx,DWORD PTR[rsp+40] ;5th arg ebx=width
mov eax,DWORD PTR[rsp+48] ;6th arg eax=height
ret
embossingFilter endp
end