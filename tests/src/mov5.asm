bits 16

; test MOV - Memory to Accumulator, Accumulator to Memory
; opcode is 1010_000w or 1010_001w

; INPUT: ax = 0x1234

; accumulator to memory (word)
mov [0x0360], ax

; memory to accumulator (word)
mov ax, [0x0360]

; accumulator to memory (byte)
mov byte [0x360], al
mov byte [0x720], ah

; memory to accumulator (byte)
mov ah, [0x360]
mov al, [0x720]

; OUTPUT
; ax = 0x3412
