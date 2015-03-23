/*
 * Simple screen routines for CoCo examples using bcc09
 *
 * Copyright 2015 by John W. Linville
 */

#include "screen.h"

void cls(unsigned char color)
{
	unsigned char *pos, val;

	if (color == 0)
		val = 0x80;
	else
		val = 0x8f | ((color - 1) << 4);

	for (pos = TXTBASE; pos < TXTEND; pos++)
		*pos = val;
}

void curpos(unsigned int val)
{
	if (val > TXTSIZE)
		abort("INVALID CURPOS!");

	*CURPOS = TXTBASE + val;
}
