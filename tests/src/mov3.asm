bits 16

; test MOV - Register/memory to/from register
; opcode is 1000_10dw
; INIT: cx = 0x1234, si = 0x0005

; move byte sized reg/reg
mov bl, cl 
mov dh, cl

; move byte sized mem/reg
mov byte [1000], cl

; move byte sized reg/mem
mov al, [1000]

; mov byte sized mem/reg w/ effaddr
mov byte [si + 1000], cl

; mov byte sized reg/mem w/ effaddr
mov ah, [si + 1000]

; OUTPUT
; ax = 0x3434
; bx = 0x0034
; cx = 0x1234
; dx = 0x3400
; [1000] = 0x34
; [1005] = 0x34
