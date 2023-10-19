bits 16
; test ADD - Immediate to register/memory, Immediate to Accumulator
; opcode is 1000_00sw and 0000_010w

; test add immediate to register
add bx, 101 ; bx == 101
add cx, 101 ; cx == 101
add cx, 101 ; cx == 202
add cl, 2	; cl == 4, cx == 204
add ch, 2	; ch == 2, cx == 716
add si, 1337; si == 1337

; test add immediate to memory
add word [0x999a], 101	; [0x999a] == 101
add word [0x999a], 234	; [0x999a] == 335

add byte [si], 4
add byte [si], 4
add byte [si], 4
add byte [si], 4	; [si] == 16

add byte [si + 8], 255	; [si + 8] == 255
add byte [si + 8], 1	; [si + 8] == 0

; add immediate to accumulator
add ax, 0x20	; ax == 0x20
add al, 0x01	; ax == 0x21
add ah, 0x01	; ax == 0x121

; OUTPUT
; ax = 0x0121
; bx = 0x0065
; cx = 0x02cc
; si = 0x0539
; word [0x999a] = 0x014f
; byte [si] = 0x10
; byte [si + 8] = 0x00
