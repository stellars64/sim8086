bits 16
; test ADD - Reg/Memory with register to either
; opcode is 0000_00dw

;             0x00ea        0x01d4  
; INPUT: ax = 234,     bx = 468

;             0x02be
; 234 + 468 = 702
add ax, bx

;             0x0492
; 468 + 702 = 1170
add bx, ax

; dl == 0x04
add dl, bh

; dh == 0x92
add dh, bl

; OUTPUT
; ax = 0x02be
; bx = 0x0492
; dx = 0x9204
