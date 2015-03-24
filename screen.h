/*
 * Header for simple screen routines for CoCo examples using bcc09
 *
 * Copyright 2015 by John W. Linville
 */

#define CURPOS		((unsigned int *)0x0088)
#define	TXTBASE		((char *)0x0400)
#define	TXTWIDTH	32
#define	TXTLINES	16
#define	TXTSIZE		((unsigned int)TXTWIDTH*TXTLINES)
#define	TXTEND		(TXTBASE+TXTSIZE)

extern void cls(unsigned char color);

extern void curpos(unsigned int val);

extern void putvdg(int val);
