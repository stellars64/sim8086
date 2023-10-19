bits 16

; test reg8/reg8 group1 arithmetic 
; ax: ADD 
; bx: SUB
; cx: ADC 
; dx: SBB
; run with:
;	al, bl = 0x47
;   ah, bh = 0x22
;	dl = 0x11

; operations on lower register

; add
add al, dl

; sub
sub bl, dl

; adc
stc
adc ah, dl

; sbb
stc
sbb bh, dl

; OUTPUT
; al = 0x58
; bl = 0x36
; ah = 0x34
; bh = 0x10
