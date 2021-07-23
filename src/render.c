#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

extern int debug;
char topText[255];
const char name[] = "Kumisweeper";
const char *diffs[] = {
	"Novice",
	"Intermediate",
	"Expert"
};

void
drawTile(int i, Board *b)
{
	char indicator[2];
	Color indicatorColors[] = {
		SKYBLUE,
		BLUE,
		DARKBLUE,
		GREEN,
		YELLOW,
		RED,
		PURPLE,
		DARKPURPLE
	};
	int fSize = b->recs[i].width;

	if (b->board[i] & REVEALED) {
		int adjMines = getAdjacentMines(i, b);

		DrawRectangleRec(b->recs[i], b->board[i] & MINED ? RED : RAYWHITE);

		if (adjMines != 0 && !(b->board[i] & MINED)) {
			sprintf(indicator, "%d", adjMines);
			DrawText(indicator, b->recs[i].x + (b->recs[i].width - MeasureText(indicator, fSize)) / 2, b->recs[i].y + 2, fSize, indicatorColors[adjMines - 1]);
		}
		return;
	}

	if (b->board[i] & FLAGGED) {
		DrawRectangleRec(b->recs[i], BLUE);
		return;
	}

	if (getHoveredTile(b) == i && b->state == PLAYING)
		DrawRectangleRec(b->recs[i], GRAY);
	else
		DrawRectangleRec(b->recs[i], LIGHTGRAY);
}

void
drawBoard(int sw, int sh, Board *b)
{
	for (int i = 0; i < b->height * b->width; ++i)
		drawTile(i, b);
}

void
drawTopBar(int sw, Board *b)
{
	sprintf(topText, "%d/%d", b->nFlagged, b->nMines);
	DrawText(topText, 5, 1, OFFSET - 2, RAYWHITE);
}

void
drawDebugOverlay(Board *b)
{
	int hovered = getHoveredTile(b);

	DrawRectangleLinesEx(b->recs[hovered], 2.0F, GREEN);

	for (int i = 0; i < 8; ++i) {
		int adj = getAdjacentTile(hovered, i, b);
		if (adj != -1)
			DrawRectangleLinesEx(b->recs[adj], 2.0f, (b->board[adj] & MINED ? RED: GREEN));
	}

	DrawText("DEBUG", 4, GetScreenHeight() - 25, 25, RED);
}

void
drawEndScreen(int sw, int sh, Board *b)
{
	DrawRectangle(0, 0, sw, sh, (Color) {200, 200, 200, 200});
	if(b->state == WON) {
		DrawText(WINTEXT, sw/2 - MeasureText(WINTEXT, 50)/2, sh/2 - 50, 50, BLACK);
	} else {
		DrawText(LOSETEXT, sw/2 - MeasureText(LOSETEXT, 50)/2, sh/2 - 50, 50, BLACK);
	}

	DrawText("Press R to go back to the main menu...", sw/2 - MeasureText("Press R to go back to the main menu...", 40)/2, sh/2, 40, BLACK);
}

void
renderDiffSelect(Rectangle *buttons)
{
	int recWidth = 0;
	int recFSize = 50;
	int titleFSize = 75;
	int buttonsYOffset = 200;
	int recHeight = recFSize + 8;
	int buttonsXOffset = 0;

	for (int i = 0; i < 3; ++i)
		if (MeasureText(diffs[i], recFSize) > recWidth)
			recWidth = MeasureText(diffs[i], recFSize);
	recWidth += 16;

	buttonsXOffset = (GetScreenWidth() - recWidth) / 2.0F;

	for (int i = 0; i < 3; ++i) {
		buttons[i].height = recHeight;
		buttons[i].width = recWidth;
		buttons[i].y = buttonsYOffset + i * (recHeight + 4);
		buttons[i].x = buttonsXOffset;
	}

	buttons[3].height = recHeight;
	buttons[3].width = recWidth;
	buttons[3].x = buttonsXOffset;
	buttons[3].y = GetScreenHeight() - recHeight - 10;

	BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawText(name, (GetScreenWidth() - MeasureText(name, titleFSize))/2, 30, titleFSize, RAYWHITE);
		for (int i = 0; i < 3; ++i) {
			DrawRectangleRec(buttons[i], GRAY);
			DrawText(diffs[i], buttons[i].x + (recWidth - MeasureText(diffs[i], recFSize))/2, buttons[i].y + 2, recFSize, RAYWHITE);
			if (getHoveredButton(buttons) == i)
				DrawRectangleLinesEx(buttons[i], 4, BLUE);
		}

		DrawRectangleRec(buttons[3], GRAY);
		DrawText("Quit", buttons[3].x + (recWidth - MeasureText("Quit", recFSize))/2, buttons[3].y + 2, recFSize, RAYWHITE);
		if (getHoveredButton(buttons) == 3)
			DrawRectangleLinesEx(buttons[3], 4, RED);
	EndDrawing();
}

void
render(Board *b)
{
	int sw = GetScreenWidth();
	int sh = GetScreenHeight();
	BeginDrawing();
		ClearBackground(DARKGRAY);
		drawBoard(sw, sh, b);
		drawTopBar(sw, b);
		if (debug)
			drawDebugOverlay(b);
		if (b->state == WON || b->state == LOST)
			drawEndScreen(sw, sh, b);
	EndDrawing();
}
