bits 16

; test MOV - Immediate to Register/Memory && Immediate to Register
; opcode is 1100_011w or 1011_wreg

; test immediate to register, word sized
mov ax, 0x1234
mov bx, 0x2345
mov cx, 0x3456
mov dx, 0x4567
mov si, 0x0002

; test immediate to register, byte sized
mov al, 0xff
mov bh, 0xff
mov cl, 0x00
mov dh, 0x00

; test immediate to direct address
mov word [0x1000], 0xf0f0
mov byte [0x2000], 0x0f

; test immediate to effective address
mov word [bx + si + 9], 0xf0f0
mov byte [bx + si + 90], 0x0f

; OUTPUT
; ax = 0x12ff
; bx = 0xff45
; cx = 0x3400
; dx = 0x0067
; si = 0x0002
; word[0x1000] = 0xf0f0
; byte[0x2000] = 0x0f
; word[0xff50] = 0xf0f0
; byte[0xffa1] = 0x0f
