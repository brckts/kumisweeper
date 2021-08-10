#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

int main(void);
void mainMenu(Rectangle *buttons, Board *b);
void cleanNClose();

int debug = 0;
Board board;
Board *b = &board;

int
main(void)
{
	InitWindow(1280, 720, "Kumisweeper");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	loadTextures();

	SetTargetFPS(30);

	b->state = MENU;

	Rectangle buttons[4] = {
		(Rectangle) {0},
		(Rectangle) {0},
		(Rectangle) {0},
		(Rectangle) {0}
	};

	while(!WindowShouldClose()) {
		switch(b->state) {
			case MENU:
				handlemenuclicks(buttons);
				renderDiffSelect(buttons);
				break;
			case PLAYING:
				updateState();
				handlekeys();
				handleclicks();
				render();
				if (IsWindowResized()) { initRecs(); }
				break;
			case WON:
			case LOST:
				render();
				handlekeys();
				if (IsWindowResized()) { initRecs(); }
				break;
			default:
				printf("SOMETHING HAS GONE HORRIBLY WRONG!\n");
				CloseWindow();
				exit(EXIT_FAILURE);
		}

	}

	cleanNClose();
}

void
cleanNClose()
{
	unloadTextures();
	UnloadFont(GetFontDefault());
	CloseWindow();
	exit(EXIT_SUCCESS);
}
