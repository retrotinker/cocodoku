	LOC	0
	ENTRY	__start
	IMPORT	__etext
	IMPORT	_main

__start:
	LDX	#0000
	PSHS	X
	LEAX	.execname,PC
	PSHS	X
	LEAX	2,S
	PSHS	X
	LEAX	2,S
	PSHS	X
	LDX	#1

	JSR	_main
	LEAS	8,S
	EXPORT	__start

	; fall-through to _exit

_exit:
	JMP	[$FFFE]
	EXPORT	_exit

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

	LOC	1
.execname:
	FCC	"ROM"
	FCB	0

	LOC	3
	IMPORT	__edata
	IMPORT	__end
