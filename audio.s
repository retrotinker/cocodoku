	LOC	0
_tap:
	ldb	#$02
	jmp	.1

_buzz:
	ldb	#$14

.1:	tst	$ff02		; Wait for vsync
	sync

	lda	$ff22		; Toggle square wave output...
	eora	#$02
	sta	$ff22

	decb
	bne	.1

	rts
	EXPORT _tap
	EXPORT _buzz
