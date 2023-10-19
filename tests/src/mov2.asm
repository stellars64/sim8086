bits 16

; test MOV - Register/memory to/from register
; opcode is 1000_10dw
; INIT: cx = 0x1234, si = 0x0005

; register to memory
mov [1000], cx		; simple address
mov [si + 1000], cx  ; effective address

; memory to register
mov ax, [1000]		; simple address
mov dx, [si + 1000]  ; effective address

; OUTPUT
; cx = 0x1234
; si = 0x0005
; ax = 0x1234
; dx = 0x1234
; [1000] = 0x1234
; [1005] = 0x1234
