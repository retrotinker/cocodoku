/*
 * CoCo sudoku game cartridge using bcc09
 *
 * Copyright 2015 by John W. Linville
 */

#define	GAMERAM		0x0600
#define GAMESIZE	9*9

#define	KEY_LEFT	0x08
#define	KEY_UP		0x5e
#define	KEY_DOWN	0x0a
#define	KEY_RIGHT	0x09
#define	KEY_BREAK	0x03

enum solver {
	MANUAL_SOLVE,
	AUTO_SOLVE,
};

char titlegame[9][9] = {
	{ 0x84,    0,    2, 0x83,    0,    6, 0x87,    0,    9, },
	{    0,    5,    0,    7,    0,    8, 0x81,    0,    0, },
	{    0, 0x88,    0,    1, 0x84,    2,    0,    0, 0x86, },
	{ 0x81,    2,    0,    0,    0, 0x85, 0x88,    0,    7, },
	{    0,    4, 0x87, 0x88,    1,    9,    0,    0,    0, },
	{    6,    0,    8,    0,    0, 0x87,    4, 0x81,    5, },
	{    2, 0x83,    5,    0,    0,    1, 0x89,    0,    0, },
	{ 0x89,    0,    0, 0x85, 0x88,    4,    0,    0,    0, },
	{    0,    6, 0x84,    0,    0,    3, 0x85,    0,    1, },
};

int puts(char *str)
{
	unsigned char c;

	while (c = *str++) {
		if (putchar(c) < 0)
			return(-1);
	}
}

void abort(char *str)
{
	puts(str);
	exit();
}

void cleargame(void)
{
	memset((char *)GAMERAM, 0, GAMESIZE);
}

void snapshotgame(void)
{
	memcpy((char *)GAMERAM+GAMESIZE, GAMERAM, GAMESIZE);
}

void restoregame(void)
{
	memcpy((char *)GAMERAM, GAMERAM+GAMESIZE, GAMESIZE);
}

void setupgame(void)
{
	char *ptr = GAMERAM;
	int i;

	for (i = 0; i < GAMESIZE; ptr++, i++)
		if (!(*ptr & 0x80))
			*ptr = 0;
}

void wrgame(int i, int j, char val)
{
	*((char *)GAMERAM + i*9 + j) = val;
}

int rdgame(int i, int j)
{
	return *((char *)GAMERAM + i*9 + j);
}

int boardpos(int i, j)
{
	int pos;

	pos = 96 + 2;
	pos += 5 * (j / 3) + (j % 3);
	pos += 32 * (4 * (i / 3) + (i % 3));

	curpos(pos);

	return pos;
}

void putboard(int i, int j, char val)
{
	boardpos(i, j);

	if (!val)
		putchar(' ');
	else
		if (val < 128)
			putchar('0' + val);
		else
			putvdg(0x30 + (val & 0x7f));
}

void showvalid(void)
{
	curpos(485);
	putvdg(0x80);
	puts("VALID");
	putvdg(0x80);
}

void showinvalid(void)
{
	curpos(485);
	puts("INVALID");
}

void drawboard(void)
{
	int i, j;
	char val;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			val = rdgame(i, j);

			putboard(i, j, val);
		}
	}
}

void drawframe(void)
{
	int i, j, pos;

	curpos(21);
	puts("COCODOKU");

	for (i = 64+2; i < 576+2; i += 128) {
		curpos(i);
		for (j = 0; j < 13; j++)
			putvdg(0xef);
	}

	for (i = 64; i < 64+20; i += 5) {
		pos = i;
		curpos(pos);
		for (j = 0; j < 13; pos += 32, j++) {
			curpos(pos);
			putvdg(0xef);
			putvdg(0xef);
		}
	}
}

int invldrow(int row, int okzero)
{
	int col;
	char val, seen[9];

	memset(seen, 0, 9);

	for (col = 0; col < 9; col++) {
		val = rdgame(row, col);
		val &= 0x7f;
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
		val = rdgame(row, col);
		val &= 0x7f;
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

		val = rdgame(bi, bj);
		val &= 0x7f;
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

int solve(int row, int column)
{
	int i, nrow, ncol, result;

	if (chkchar() == KEY_BREAK)
		return -1;

	if (column != 8) {
		nrow = row;
		ncol = column + 1;
	} else {
		nrow = row + 1;
		ncol = 0;
	}

	if (!rdgame(row, column)) {
		for (i = 1; i <= 9; i++) {
			wrgame(row, column, i);
			putboard(row, column, i);

			if (invalid(1))
				continue;

			if (nrow == 9)
				return 1;

			result = solve(nrow, ncol);
			if (result != 0)
				return result;
		}

		wrgame(row, column, 0);
		putboard(row, column, 0);
		return 0;
	}

	if (nrow == 9)
		return 1;
	else
		return solve(nrow, ncol);
}

void showtitle(void)
{
	cls(0);

	memcpy(GAMERAM, titlegame, sizeof(titlegame));

	drawframe();
	drawboard();

	curpos(114);
	puts("NUMERIC PUZZLE");

	curpos(148);
	puts("LOGIC GAME");

	curpos(276);
	puts("PROGRAM BY");

	curpos(306);
	puts("J. W. LINVILLE");

	curpos(341);
	puts("(C) 2015");

	curpos(467);
	puts("PRESS ANY KEY");
	curpos(500);
	puts("TO CONTINUE");

	while (chkchar() == -1) {}
}

enum solver editboard(void)
{
	int val, scnval, pos;
	int i = 0, j = 0, k;

	cls(0);

	curpos(34);
	puts("PUZZLE EDITOR");

	curpos(85);
	putvdg(0x13);
	puts("NAPSHOT");

	curpos(117);
	putvdg(0x12);
	puts("ESTORE ");

	curpos(149);
	putvdg(0x0e);
	puts("EW GAME");

	curpos(212);
	putvdg(0x01);
	puts("UTO SOLVER");

	curpos(244);
	putvdg(0x0d);
	puts("ANUAL PLAY");

	curpos(339);
	puts("USE ARROWS TO");
	curpos(372);
	puts("MOVE AROUND");

	curpos(434);
	puts("USE NUMBERS TO");
	curpos(467);
	puts("SETUP PUZZLE");

	setupgame();
	snapshotgame();

	drawframe();
	drawboard();

	for (;;) {
		pos = boardpos(i, j);

		if (scnval = rdgame(i, j))
			putvdg(0x70 + (scnval & 0x7f));
		else
			putvdg(0x20);
		do {
			val = chkchar();
		} while (val == -1);
		curpos(pos);
		if (scnval = rdgame(i, j))
			putvdg(0x30 + (scnval & 0x7f));
		else
			putchar(' ');

		curpos(pos);
		if (val == KEY_LEFT) {
			if (j)
				j--;
		} else if (val == KEY_RIGHT) {
			if (j < 8)
				j++;
		} else if (val == KEY_UP) {
			if (i)
				i--;
		} else if (val == KEY_DOWN) {
			if (i < 8)
				i++;
		} else if ((val >= '0') && (val <= '9')) {
			val -= '0';
			if (!val)
				wrgame(i, j, 0);
			else
				wrgame(i, j, 0x80 + val);

			if (invalid(1))
				showinvalid();
			else
				showvalid();

			curpos(pos);
			if (!val)
				putchar(' ');
			else
				putvdg(0x30 + val);
		} else if (val == 'N') {
			curpos(480);
			puts("CLEAR PUZZLE? (Y/N)");

			do {
				val = chkchar();
			} while ((val == -1) && (val != 'Y') && (val != 'N'));

			curpos(480);
			for (k = 0; k < 19; k++)
				putvdg(0x80);

			if (val == 'Y') {
				cleargame();
				drawboard();
			} else {
				if (invalid(1))
					showinvalid();
				else
					showvalid();
			}
		} else if (val == 'R') {
			restoregame();
			drawboard();
		} else if (val == 'S') {
			snapshotgame();
		} else if (val == 'M') {
			if (invalid(1))
				continue;
			return MANUAL_SOLVE;
		} else if (val == 'A') {
			if (invalid(1))
				continue;
			return AUTO_SOLVE;
		}
	}
}

void playboard(void)
{
	int val, scnval, pos;
	int i = 0, j = 0, k;

	cls(0);

	curpos(35);
	puts("MANUAL PLAY");

	curpos(117);
	putvdg(0x13);
	puts("NAPSHOT");

	curpos(149);
	putvdg(0x12);
	puts("ESTORE ");

	curpos(213);
	putvdg(0x11);
	puts("UIT GAME");

	curpos(339);
	puts("USE ARROWS TO");
	curpos(372);
	puts("MOVE AROUND");

	curpos(434);
	puts("USE NUMBERS TO");
	curpos(467);
	puts("SOLVE PUZZLE");

	setupgame();
	snapshotgame();

	drawframe();
	drawboard();

	for (;;) {
		pos = boardpos(i, j);

		if (scnval = rdgame(i, j))
			putvdg(0x40 * !!(scnval & 0x80) + 0x30 + (scnval & 0x7f));
		else
			putvdg(0x20);
		do {
			val = chkchar();
		} while (val == -1);
		curpos(pos);
		if (scnval = rdgame(i, j))
			putvdg(0x40 * !(scnval & 0x80) + 0x30 + (scnval & 0x7f));
		else
			putchar(' ');

		curpos(pos);
		if (val == KEY_LEFT) {
			if (j)
				j--;
		} else if (val == KEY_RIGHT) {
			if (j < 8)
				j++;
		} else if (val == KEY_UP) {
			if (i)
				i--;
		} else if (val == KEY_DOWN) {
			if (i < 8)
				i++;
		} else if ((val >= '0') && (val <= '9')) {
			if (rdgame(i, j) & 0x80)
				continue;

			val -= '0';
			wrgame(i, j, val);

			if (invalid(1))
				showinvalid();
			else
				showvalid();

			curpos(pos);
			if (!val)
				putchar(' ');
			else
				putvdg(0x70 + val);
		} else if (val == 'S') {
			snapshotgame();
		} else if (val == 'R') {
			restoregame();
			drawboard();
		} else if (val == 'Q') {
			break;
		}
	}
}

void solveboard(void)
{
	int result;

	cls(0);

	curpos(35);
	puts("AUTO SOLVER");

	setupgame();

	curpos(242);
	puts("USE ");
	putvdg(0x02);
	putvdg(0x12);
	putvdg(0x05);
	putvdg(0x01);
	putvdg(0x0b);
	puts(" KEY ");
	curpos(274);
	puts("TO EXIT SOLVER");

	drawframe();
	drawboard();

	result = solve(0, 0);
	if (result == -1)
		return;

	if (result)
		showvalid();
	else
		showinvalid();

	while (chkchar() != KEY_BREAK) {}
}

void main(int argc, char *argv)
{
	enum solver result;

	showtitle();

	cleargame();

	for (;;) {
		result = editboard();

		switch (result) {
		case MANUAL_SOLVE:
			playboard();
			break;

		case AUTO_SOLVE:
			solveboard();
			break;
		}
	}
}
