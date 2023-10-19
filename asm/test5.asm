bits 16

mov cx, 0x10
LOOP_START:
	add ax, cx
	sub cx, 1
	jnz LOOP_START
