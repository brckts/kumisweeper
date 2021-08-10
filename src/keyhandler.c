#include "minesweeper.h"
#include "raylib.h"

extern int debug;
extern Board *b;

void
handlekeys()
{
	switch(GetKeyPressed()) {
		case KEY_R:
			if (b->state != MENU) {
				freeBoard();
			}
			b->state = MENU;
			break;
		case KEY_D:
			debug = !debug;
			break;
	}
}
