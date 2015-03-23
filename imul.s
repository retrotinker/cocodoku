	loc	0
imul:
imulu:
	pshs	d,x,y

	lda	3,s
	mul		; LSB1 * LSB2
	std	4,s
	lda	2,s
	ldb	1,s
	mul		; LSB1 * MSB2
	addb	4,s
	stb	4,s
	lda	3,s
	ldb	0,s
	mul		; MSB1 * LSB2
	addb	4,s
	tfr	b,a
	ldb	5,s
	leas	6,s
	rts
	export	imul
	export	imulu
