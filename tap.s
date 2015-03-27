	LOC	0
_tap:
	pshs	d

	ldb	#$02

.1:	tst	$ff02		; Wait for vsync
	sync

	lda	$ff22		; Toggle square wave output...
	eora	#$02
	sta	$ff22

	decb
	bne	.1

	puls	d,pc
	EXPORT _tap
