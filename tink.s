	LOC	0
_tink:
	pshs	d

	ldb	#$20

.1:	tst	$ff00		; Wait for hsync
	sync

	lda	$ff22		; Toggle square wave output...
	eora	#$02
	sta	$ff22

	decb
	bne	.1

	puls	d,pc
	EXPORT _tink
