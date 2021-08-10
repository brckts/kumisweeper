#include "minesweeper.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <raymath.h>

enum Textures {
	MINE,
	FLAG,
	LOGO
};

extern Board *b;
extern int debug;
char topText[255];
const char name[] = "DEKOUMIKEUR";
const char retry[] = "REVIEN O MENU AVEK R";
const char loseText[] = "TA PERDU!";
const char winText[] = "TA GAGNER!";
const char quitText[] = "KITER";
const char *diffs[] = {
	"FACIL",
	"MOYAN",
	"DUR"
};

const Color indicatorColors[] = {
	SKYBLUE,
	BLUE,
	DARKBLUE,
	GREEN,
	YELLOW,
	RED,
	PURPLE,
	DARKPURPLE
};

Texture2D textures[3];

void
loadTextures(void)
{
	textures[MINE] = LoadTexture("src/resources/patoche_transparent.png");
	textures[FLAG] = LoadTexture("src/resources/koin.png");
	textures[LOGO] = LoadTexture("src/resources/patoche_transparent.png");
}

void
unloadTextures(void)
{
	for (int i = 0; i < 3; ++i)
		UnloadTexture(textures[i]);
}

void
drawHitMarker(Vector2 pos)
{
	int innerRadius = 10;
	int outerRadius = 30;
	float angle;
	Vector2 start;
	Vector2 end;

	for (int i = 0; i < 4; ++i) {
		angle = PI / 2.0F * i + PI / 4.0F;
		start = Vector2Add(pos, (Vector2) { cosf(angle) * innerRadius, sinf(angle) * innerRadius});
		end = Vector2Add(pos, (Vector2) { cosf(angle) * outerRadius, sinf(angle) * outerRadius});
		DrawLineEx(start, end, 10.0F, GOLD);
	}
}

void
drawTextureTile(Texture2D texture, Rectangle rec)
{
	DrawTextureEx(texture, (Vector2) {rec.x, rec.y}, 0.0F, (float) (rec.width) / 128.0F, WHITE);
}

void
drawTile(int i)
{
	char indicator[2];
	int fSize = b->recs[i].width;

	if (b->board[i] & REVEALED) {
		int adjMines = getAdjacentMines(i);

		if (b->board[i] & MINED) {
			DrawRectangleRec(b->recs[i], RED);
			drawTextureTile(textures[MINE], b->recs[i]);
		} else {
			DrawRectangleRec(b->recs[i], RAYWHITE);
		}

		if (adjMines != 0 && !(b->board[i] & MINED)) {
			sprintf(indicator, "%d", adjMines);
			DrawText(indicator, b->recs[i].x + (b->recs[i].width - MeasureText(indicator, fSize)) / 2, b->recs[i].y + 2, fSize, indicatorColors[adjMines - 1]);
		}
		return;
	}

	if (b->board[i] & FLAGGED) {
		DrawRectangleRec(b->recs[i], BLUE);
		drawTextureTile(textures[FLAG], b->recs[i]);
		return;
	}

	if (getHoveredTile(b) == i && b->state == PLAYING)
		DrawRectangleRec(b->recs[i], GRAY);
	else
		DrawRectangleRec(b->recs[i], LIGHTGRAY);
}

void
drawBoard(int sw, int sh)
{
	for (int i = 0; i < b->height * b->width; ++i)
		drawTile(i);
}

void
drawTopBar(int sw)
{
	sprintf(topText, "%d/%d", b->nFlagged, b->nMines);
	DrawText(topText, 5, 1, OFFSET - 2, RAYWHITE);
}

void
drawDebugOverlay()
{
	int hovered = getHoveredTile(b);

	DrawRectangleLinesEx(b->recs[hovered], 2.0F, GREEN);

	for (int i = 0; i < 8; ++i) {
		int adj = getAdjacentTile(hovered, i);
		if (adj != -1)
			DrawRectangleLinesEx(b->recs[adj], 2.0f, (b->board[adj] & MINED ? RED: GREEN));
	}

	DrawText("DEBUG", 4, GetScreenHeight() - 25, 25, RED);
}

void
drawEndScreen(int sw, int sh)
{
	DrawRectangle(0, 0, sw, sh, (Color) {200, 200, 200, 200});
	if(b->state == WON) {
		DrawText(winText, sw/2 - MeasureText(winText, 50)/2, sh/2 - 50, 50, BLACK);
	} else {
		DrawText(loseText, sw/2 - MeasureText(loseText, 50)/2, sh/2 - 50, 50, BLACK);
	}

	DrawText(retry, sw/2 - MeasureText(retry, 40)/2, sh/2, 40, BLACK);
}

void
drawRotatingTexture(float speed, Vector2 pos, Texture2D texture)
{
	int rotCircleRadius = sqrt(2) * 64;
	Vector2 circlePos = { pos.x + 64, pos.y + 64 };
	float logoRot = (int)(GetTime() * speed) % 360;
	float angle = DEG2RAD * logoRot - (3.0F / 4.0F) * PI;
	Vector2 logoPos = Vector2Add(circlePos, (Vector2) {cosf(angle) * rotCircleRadius, sinf(angle) * rotCircleRadius});
	DrawTextureEx(texture, logoPos, logoRot, 1.0F, WHITE);
}

void
drawTitle(int titleFSize)
{
	int txtWidth = MeasureText(name, titleFSize);
	int titleX = (GetScreenWidth() - txtWidth)/2;

	Vector2 logoPos = { titleX - 180, 30 };


	DrawText(name, titleX, 30, titleFSize, RAYWHITE);
	drawRotatingTexture(40.0F, logoPos, textures[LOGO]);
	drawRotatingTexture(-40.0F, (Vector2) { titleX + txtWidth + 52, 30 }, textures[LOGO]);
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
		drawTitle(titleFSize);
		for (int i = 0; i < 3; ++i) {
			DrawRectangleRec(buttons[i], GRAY);
			DrawText(diffs[i], buttons[i].x + (recWidth - MeasureText(diffs[i], recFSize))/2, buttons[i].y + 2, recFSize, RAYWHITE);
			if (getHoveredButton(buttons) == i)
				DrawRectangleLinesEx(buttons[i], 4, BLUE);
		}

		DrawRectangleRec(buttons[3], GRAY);
		DrawText(quitText, buttons[3].x + (recWidth - MeasureText(quitText, recFSize))/2, buttons[3].y + 2, recFSize, RAYWHITE);
		if (getHoveredButton(buttons) == 3)
			DrawRectangleLinesEx(buttons[3], 4, RED);
	EndDrawing();
}

void
render()
{
	int sw = GetScreenWidth();
	int sh = GetScreenHeight();
	BeginDrawing();
		ClearBackground(DARKGRAY);
		drawBoard(sw, sh);
		drawTopBar(sw);
		if (debug)
			drawDebugOverlay();
		if (b->state == WON || b->state == LOST)
			drawEndScreen(sw, sh);
	EndDrawing();
}
