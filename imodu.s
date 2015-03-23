	loc	0
imodu:			; D is dividend, X is divisor
	pshs	d,x
	clra
	clrb
	ldx	#$0011	; loop for 17 iterations
.1:
	andcc	#$fe	; clear carry
.2:
	rol	1,s	; rotate dividend/result
	rol	0,s
	leax	-1,x	; decrement loop count
	beq	.3	; exit if zero
	rolb		; rotate dividend/result
	rola
	cmpd	2,s	; divisor is less than dividend/result?
	blo	.1	; rotate in a zero and continue
	subd	2,s	; reduce dividend/result by divisor
	orcc	#$01	; rotate in a one and continue
	bra	.2
.3:
	ldx	0,s	; X = D / X, D = D % X
	leas	4,s
	rts		; result returned in D
	export	imodu
