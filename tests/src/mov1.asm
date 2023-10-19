bits 16

; test MOV - Register/memory to/from register
; opcode is 1000_10dw
; INIT: cx = 0x1234

; register to register 16 bit moves
mov bx, cx
mov dx, bx
; cx=0x1234
; bx=0x1234
; dx=0x1234

; register to register 8 bit moves
mov al, cl
; ax=0x0034

mov bh, cl
; bx=0x3434

; OUTPUT
; ax=0x0034
; bx=0x3434
; cx=0x1234
; dx=0x1234
