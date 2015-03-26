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

	ORCC	#$50
	CLR	$FFD7

	lda	$ff03		; Disable vsync interrupt generation
	anda	#$fc
	sta	$ff03
	tst	$ff02
	ldb	$ff01		; Enable hsync interrupt generation
	orb	#$01
	stb	$ff01

	lda     $ff23		; Enable square wave audio output
	anda    #$fb
	sta     $ff23
	ldb     #$02
	orb     $ff22
	stb     $ff22
	ora     #$04
	sta     $ff23

	JSR	_main
	LEAS	8,S
	EXPORT	__start

	; fall-through to _exit

_exit:
	JMP	[$FFFE]
	EXPORT	_exit

	LOC	1
.execname:
	FCC	"ROM"
	FCB	0

	LOC	3
	IMPORT	__edata
	IMPORT	__end
