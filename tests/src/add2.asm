bits 16
; test ADD - Reg/Memory with register to either, memory
; opcode is 0000_00dw

;             0x00ea        0x01d4  
; INPUT: ax = 234,     bx = 468,     si = 27

; add mem/reg, then reg/mem
add [0x1000], ax
add bx, [0x1000] ; bx = 0x02be

; add mem/reg, then reg/mem w/ effective addresses
add [si + 101], bx	; [si+101]=0x00ea
add [si + 101], ax	; [si+101]=(0x00ea + 0x02be) = 0x3a8
add dx, [si + 101]	; dx=0x3a8
add dx, [si + 101]	; dx=(0x3a8 + 0x3a8) = 0x750

; OUTPUT
; ax = 0x00ea
; bx = 0x02be
; dx = 0x0750
; [0x1000] = 0x00ea
; [128] = 0x03a4
