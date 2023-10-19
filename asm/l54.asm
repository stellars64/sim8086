; my version of listing_0054

bits 16

mov dx, 0

y_loop:
	mov cx, 0
	x_loop:
		; draw pixel
		; -- ch = Red
		; -- cl = Green
		; -- dx = Blue
		; -- alpha is contantly 0xFF
		mov byte [bp + 0], ch
		mov byte [bp + 1], cl
		mov byte [bp + 2], dl
		mov byte [bp + 3], 255
		; advance pixel location
		add bp, 4
		; advance x
		add cx, 1
		cmp cx, 64
		jnz x_loop
	
	; advance y
	add dx, 1
	cmp dx, 64
	jnz y_loop
