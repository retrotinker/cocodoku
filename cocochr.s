	LOC	0
_putchar:
	TFR	X,D
	EXG	A,B
	CMPA	#$0A
	BNE	.1
	JSR	[$A002]
	LDA	#$0D
.1:
	JSR	[$A002]
	RTS
	EXPORT	_putchar

_chkchar:
	JSR	[$A000]
	BNE	.2
	LDX	#$FFFF
	RTS
	EXPORT	_chkchar

_getchar:
	JSR	[$A000]
	BEQ	_getchar
.2:	TFR	A,B
	CLRA
	CMPB	#$0D
	BNE	.3
	LDB	#$0A
.3:
	TFR	D,X
	RTS
	EXPORT	_getchar
