#include "minesweeper.h"
#include "raylib.h"

extern int debug;
void
handlekeys(Board *b)
{
	switch(GetKeyPressed()) {
		case KEY_R:
			if (b->state != MENU) {
				freeBoard(b);
			}
			b->state = MENU;
			break;
		case KEY_D:
			debug = !debug;
			break;
	}
}
