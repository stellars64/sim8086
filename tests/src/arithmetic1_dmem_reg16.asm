bits 16

; test dmem/reg16 group1 arithmetic 
; [1000]: ADD 
; [1002]: SUB
; [1004]: ADC 
; [1006]: SBB
;
; run with:
;	[1000],[1002],[1004],[1006] = 50177
;	si == 9131

; add
add [1000], si

; sub
sub [1002], si

; adc
stc
adc [1004], si

; sbb
stc
sbb [1006], si
