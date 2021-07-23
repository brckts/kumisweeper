#include "minesweeper.h"
#include <stdlib.h>

void
handleclicks(Board *b)
{
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		int hoveredTile = getHoveredTile(b);
		if (hoveredTile != -1)
			revealTile(hoveredTile, b);
	}

	if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		int hoveredTile = getHoveredTile(b);
		if (hoveredTile != -1)
			flagTile(hoveredTile, b);
	}
}

void
handlemenuclicks(Rectangle *buttons, Board *b)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		int hoveredButton = getHoveredButton(buttons);
		switch(hoveredButton) {
			case NOVICE:
			case INTERMEDIATE:
			case EXPERT:
				initBoard(hoveredButton, b);
				initRecs(b);
				break;
			case 3:
				CloseWindow();
				exit(EXIT_SUCCESS);
				break;
			default:
				return;
		}
	}
}

int
getHoveredButton(Rectangle *buttons)
{
	Vector2 pos = GetMousePosition();
	for (int i = 0; i < 4; ++i) {
		if (CheckCollisionPointRec(pos, buttons[i]))
			return i;
	}

	return -1;
}

int
getHoveredTile(Board *b)
{
	Vector2 pos = GetMousePosition();
	for (int i = 0; i < b->height * b->width; ++i) {
		if (CheckCollisionPointRec(pos, b->recs[i]))
			return i;
	}

	return -1;
}
