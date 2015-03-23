	loc	0
; x   -> dest ->	0,s
; 2,s -> src  ->	8,s
; 4,s -> len  ->	10,s
_memcpy:
	pshs	x,y,u	; only need to save/restore y,u if -C-f-
	ldu	8,s

	lda	11,s	; take care of odd length values
	bita	#$01
	beq	.1
	lda	,u+
	sta	,x+
	dec	11,s	; decrement length by 1
	bne	.1
	tst	10,s	; exit if it goes to zero
	beq	.3

.1:			; length should be even and non-zero
	ldy	10,s
.2:
	ldd	,u++	; move 2 bytes of data at a time
	std	,x++
	leay	-2,y	; reduce remaining length count by 2
	bne	.2
.3:
	puls	x,y,u,pc
	export	_memcpy
