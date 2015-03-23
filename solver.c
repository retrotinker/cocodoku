/*
 * CoCo sudoku solver example using bcc09
 *
 * Copyright 2015 by John W. Linville
 */

#define BOARDRAM	0x0800

char startboard[9][9] = {
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	{ 6, 4, 0, 0, 0, 0, 0, 0, 0 },
	{ 7, 0, 5, 0, 0, 0, 0, 0, 0 },
	{ 8, 0, 0, 6, 0, 0, 0, 0, 0 },
	{ 9, 0, 0, 0, 7, 0, 0, 0, 0 },
	{ 2, 0, 0, 0, 0, 8, 0, 0, 0 },
	{ 3, 0, 0, 0, 0, 0, 9, 0, 0 },
	{ 4, 0, 0, 0, 0, 0, 0, 2, 0 },
	{ 5, 0, 0, 0, 0, 0, 0, 0, 3 },
};

int putstr(char *str)
{
	unsigned char c;

	while (c = *str++) {
		if (putchar(c) < 0)
			return(-1);
	}
}

void abort(char *str)
{
	putstr(str);
	exit();
}

int chkchr() {
	return -1;
}

void wrboard(int i, int j, char val)
{
	*((char *)BOARDRAM + i*9 + j) = val;
}

int rdboard(int i, int j) {
	return *((char *)0x0800 + i*9 + j);
}

void setupboard(char *addr) {
	int i, j;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			wrboard(i, j, *addr++);
};

void drawboard(void) {
	int i, j, pos;
	char val;

	pos = 96+9;
	curpos(pos);

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			val = rdboard(i, j);
			if (!val)
				putchar(' ');
			else
				putchar('0' + val);
			pos++;
			if ((j % 3) == 2)
				pos += 2;
			curpos(pos);
		}
		pos += (32 - 15);
		if ((i % 3) == 2)
			pos += 32;
		curpos(pos);
	}
}

void drawframe(void) {
	int i, j, pos;

	cls(0);

	curpos(12);
	putstr("SUDOKU!");

	for (i = 64+9; i < 576+9; i += 128) {
		curpos(i);
		for (j = 0; j < 13; j++)
			putchar(0xef);
	}

	for (i = 64+7; i < 64+27; i += 5) {
		pos = i;
		curpos(pos);
		for (j = 0; j < 13; pos += 32, j++) {
			curpos(pos);
			putchar(0xef);
			putchar(0xef);
		}
	}
}

int invldrow(int row, int okzero)
{
	int col;
	char val, seen[9];

	memset(seen, 0, 9);

	for (col = 0; col < 9; col++) {
		val = rdboard(row, col);
		if (!val)
			if (okzero)
				continue;
			else
				return 1;
		if (seen[val-1])
			return 1;
		else
			seen[val-1] = 1;
	}

	return 0;
}

int invldcol(int col, int okzero)
{
	int row;
	char val, seen[9];

	memset(seen, 0, 9);

	for (row = 0; row < 9; row++) {
		val = rdboard(row, col);
		if (!val)
			if (okzero)
				continue;
			else
				return 1;
		if (seen[val-1])
			return 1;
		else
			seen[val-1] = 1;
	}

	return 0;
}

int invldblock(int i, int okzero)
{
	int j, bi, bj;
	char val, seen[9];

	memset(seen, 0, 9);

	for (j = 0; j < 9; j++) {
		bi = (3 * (i / 3)) + (j / 3);
		bj = (3 * (i % 3)) + (j % 3);

		val = rdboard(bi, bj);
		if (!val)
			if (okzero)
				continue;
			else
				return 1;
		if (seen[val-1])
			return 1;
		else
			seen[val-1] = 1;
	}

	return 0;
}

int invalid(int okzero)
{
	int i;

	/*
	 * Check each row...
	 */
	for (i = 0; i < 9; i++)
		if (invldrow(i, okzero))
			return 1;

	/*
	 * Check each column...
	 */
	for (i = 0; i < 9; i++)
		if (invldcol(i, okzero))
			return 1;

	/*
	 * Check each block...
	 */
	for (i = 0; i < 9; i++)
		if (invldblock(i, okzero))
			return 1;

	return 0;
}

int solve(int row, int column) {
	int i, nrow, ncol;

	if (chkchr() != -1)
		exit();

	if (column != 8) {
		nrow = row;
		ncol = column + 1;
	} else {
		nrow = row + 1;
		ncol = 0;
	}

	if (!rdboard(row, column)) {
		for (i = 1; i <= 9; i++) {
			wrboard(row, column, i);

			drawboard();

			if (invalid(1))
				continue;

			if (nrow == 9)
				return 1;

			if (solve(nrow, ncol))
				return 1;
		}

		wrboard(row, column, 0);
		return 0;
	}

	if (nrow == 9)
		return 1;
	else
		return solve(nrow, ncol);
}

void main(int argc, char *argv)
{
	char *setupdata = startboard;

	drawframe();

	setupboard(setupdata);

	drawboard();

	if (solve(0, 0)) {
		curpos(493);
		putstr("VALID");
	} else {
		curpos(492);
		putstr("INVALID");
	}

	while (1) {};
}
