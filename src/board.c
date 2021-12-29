#include "minesweeper.h"
#include "stdlib.h"
#include <stdio.h>

extern Board *b;
void
initBoard(enum difficulty diff)
{
	switch(diff) {
		case NOVICE:
			b->width = 8;
			b->height = 8;
			b->nMines = 10;
			break;
		case INTERMEDIATE:
			b->width = 16;
			b->height = 16;
			b->nMines = 40;
			break;
		case EXPERT:
			b->width = 30;
			b->height = 16;
			b->nMines = 99;
			break;
		default:
			b->width = 0;
			b->height = 0;
			b->nMines = 0;
	}

	b->state = PLAYING;
	b->nFlagged = 0;

	b->board = malloc(sizeof(unsigned char) * b->height * b->width);
	b->recs = malloc(sizeof(Rectangle) * b->height * b->width);
	for (int i = 0; i < b->height * b->width; ++i)
		b->board[i] = 0;
	for (int i = 0; i < b->nMines; ++i) {
		int index = rand() % (b->height * b->width);
		if (b->board[index] & MINED) {
			--i;
			continue;
		}

		b->board[index] = MINED;
	}
}

void
initRecs()
{
	int deltax = GetScreenWidth()/b->width;
	int deltay = (GetScreenHeight() - OFFSET)/b->height;
	int dim = (deltax > deltay ? deltay : deltax);
	int tileXOffset = (GetScreenWidth() - dim * b->width)/2;
	for (int i = 0; i < b->height; ++i) {
		for (int j = 0; j < b->width; ++j) {
			b->recs[j + b->width * i] = (Rectangle) {j * dim + 1 + tileXOffset, i * dim + 1 + OFFSET, dim - 2, dim - 2};
		}
	}
}

void
freeBoard()
{
	free(b->board);
	free(b->recs);
}

void
flagTile(int i)
{
	b->board[i] = b->board[i] ^ FLAGGED;
	b->nFlagged += (b->board[i] & FLAGGED) ? 1 : -1;
}

void
revealTile(int i)
{
	int adjMines = getAdjacentMines(i);
	int adjFlags = getAdjacentFlags(i);


	if (b->board[i] & FLAGGED)
		return;

	b->board[i] = b->board[i] | REVEALED;


	if (b->board[i] & MINED)
		return;

	if (adjMines == 0 || adjMines == adjFlags) {
		for (int dir = 0; dir < 8; ++dir) {
			int currAdj = getAdjacentTile(i, (enum direction) dir);
			if (currAdj > -1 && !(b->board[currAdj] & FLAGGED || b->board[currAdj] & REVEALED))
				revealTile(currAdj);
		}
	}
}

int
getAdjacentMines(int i)
{
	int mines = 0;

	for (int j = 0; j < 8; ++j)
		if (getAdjacentTile(i, (enum direction) j) != -1)
			mines += (b->board[getAdjacentTile(i, (enum direction) j)] & MINED);

	return mines;
}

int
getAdjacentFlags(int i)
{
	int flags = 0;

	for (int j = 0; j < 8; ++j)
		if (getAdjacentTile(i, (enum direction) j) != -1)
			flags += !!(b->board[getAdjacentTile(i, (enum direction) j)] & FLAGGED);

	return flags;
}

int
getAdjacentTile(int i, enum direction dir)
{
	if (i == -1)
		return -1;

	switch(dir) {
		case NORTH:
			if ((i / b->width) > 0)
				return i - b->width;
			break;
		case NORTH_EAST:
			return getAdjacentTile(getAdjacentTile(i, NORTH), EAST);
			break;
		case EAST:
			if ((i % b->width) < (b->width - 1))
				return i + 1;
			break;
		case SOUTH_EAST:
			return getAdjacentTile(getAdjacentTile(i, SOUTH), EAST);
			break;
		case SOUTH:
			if ((i / b->width) < (b->height - 1))
				return i + b->width;
			break;
		case SOUTH_WEST:
			return getAdjacentTile(getAdjacentTile(i, SOUTH), WEST);
			break;
		case WEST:
			if ((i % b->width) > 0)
				return i - 1;
			break;
		case NORTH_WEST:
			return getAdjacentTile(getAdjacentTile(i, NORTH), WEST);
			break;
		default:
			return -1;
	}

	return -1;
}

void
updateState()
{

	for (int i = 0; i < b->width * b->height; ++i) {
		if (b->board[i] & MINED && b->board[i] & REVEALED) {
			b->state = LOST;
			return;
		}
	}


	for (int i = 0; i < b->width * b->height; ++i) {
		if (
				(b->board[i] & MINED && !(b->board[i] & FLAGGED)) ||
				(!(b->board[i] & MINED) && !(b->board[i] & REVEALED))
		   )
			return;
	}

	b->state = WON;
}
