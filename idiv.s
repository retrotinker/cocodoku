	loc	0
	import	idivu
idiv:			; D is dividend, X is divisor
	pshs	d,x

	leas	-1,s
	clr	0,s	; clear sign flag

	tst	1,s	; check for sign of 1st argument
	bpl	.1
	com	0,s	; set sign-mismatch flag

	ldd	1,s	; convert 1st arg to absolute value
	coma
	comb
	addd	#$0001
	std	1,s
.1:
	tst	3,s	; check for sign of 2nd argument
	bpl	.2
	com	0,s	; toggle sign-mismatch flag

	ldd	3,s	; convert 2nd arg to absolute value
	coma
	comb
	addd	#$0001
	tfr	d,x
	ldd	1,s
.2:
	lbsr	idivu

	tst	0,s	; check sign-mismatch flag
	beq	.3

	coma		; convert result to negative value
	comb
	addd	#$0001
.3:

	leas	5,s
	rts		; result returned in D
	export	idiv
