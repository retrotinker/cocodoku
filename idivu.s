	loc	0
	import	imodu
idivu:			; D is dividend, X is divisor
	lbsr	imodu
	tfr	x,d
	rts		; result returned in D
	export	idivu
