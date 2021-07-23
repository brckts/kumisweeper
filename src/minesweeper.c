#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

int main(void);
void mainMenu(Rectangle *buttons, Board *b);
void cleanNClose(Board *b);

int debug = 0;

int
main(void)
{
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Kumisweeper");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(30);

	Board board;
	Board *b = &board;
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
				handlemenuclicks(buttons, b);
				renderDiffSelect(buttons);
				break;
			case PLAYING:
				updateState(b);
				handlekeys(b);
				handleclicks(b);
				render(b);
				if (IsWindowResized()) { initRecs(b); }
				break;
			case WON:
			case LOST:
				render(b);
				handlekeys(b);
				if (IsWindowResized()) { initRecs(b); }
				break;
			default:
				printf("SOMETHING HAS GONE HORRIBLY WRONG!\n");
				CloseWindow();
				exit(EXIT_FAILURE);
		}

	}

	cleanNClose(b);
}

void
cleanNClose(Board *b)
{
	UnloadFont(GetFontDefault());
	CloseWindow();
	exit(EXIT_SUCCESS);
}
