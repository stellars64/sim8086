bits 16

; test reg16/reg16 group1 arithmetic 
; ax: ADD 
; bx: SUB
; cx: ADC 
; dx: SBB
; run with ax,bx,cx,dx == 50177 and si == 9131

; add
add ax, si

; or
sub bx, si

; adc
stc
adc cx, si

; sbb
stc
sbb dx, si
