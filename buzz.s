	LOC	0
_buzz:
	pshs	d

	ldb	#$14

.1:	tst	$ff02		; Wait for vsync
	sync

	lda	$ff22		; Toggle square wave output...
	eora	#$02
	sta	$ff22

	decb
	bne	.1

	puls	d,pc
	EXPORT _buzz
