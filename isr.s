	loc	0
isr:
	cmpx	#$0000
	beq	.3
	cmpx	#$000f
	bgt	.2
.1:
	asra
	rorb
	leax	-1,x
	bne	.1
	rts
.2:
	ldd	#$0000
.3:
	rts
	export	isr
