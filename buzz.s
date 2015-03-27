	LOC	0
_buzz:
	pshs	d

	ldb	#$14

.1:	lda	#$00
.2:	tst	$ff00		; Wait for hsync
	sync

	deca
	bne	.2

	lda	$ff22		; Toggle square wave output...
	eora	#$02
	sta	$ff22

	decb
	bne	.1

	puls	d,pc
	EXPORT _buzz
