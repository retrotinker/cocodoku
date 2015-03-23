	loc	0
; x   -> dest ->	0,s
; 2,s -> data  ->	6,s
; 4,s -> len  ->	8,s
_memset:
	pshs	x,y	; only need to save/restore y,u if -C-f-

	ldd	8,s	; test for zero length
	beq	.3

	ldb	7,s	; load data for writing

	lda	9,s	; take care of odd length values
	bita	#$01
	beq	.1
	stb	,x+
	dec	9,s	; decrement length by 1
	bne	.1
	tst	8,s	; exit if it goes to zero
	beq	.3

.1:			; length should be even and non-zero
	ldy	8,s
	tfr	b,a	; prepare data for writing 2 bytes at a time
.2:
	std	,x++	; write 2 bytes of data at a time
	leay	-2,y	; reduce remaining length count by 2
	bne	.2
.3:
	puls	x,y,pc
	export	_memset
