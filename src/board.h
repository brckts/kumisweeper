#define MINED 1U
#define REVEALED 1U << 1
#define FLAGGED 1U << 2
#define OFFSET 30

enum difficulty {
	NOVICE,
	INTERMEDIATE,
	EXPERT
};

enum direction {
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

enum state {
	MENU,
	PLAYING,
	WON,
	LOST
};

typedef struct board {
	unsigned char *board;
	Rectangle *recs;
	unsigned width;
	unsigned height;
	unsigned nMines;
	unsigned nFlagged;
	enum state state;
} Board;

void initBoard(enum difficulty diff);
void freeBoard();
void initRecs();
void revealTile(int i);
void flagTile(int i);
int getAdjacentTile(int i, enum direction dir);
int getAdjacentMines(int i);
int getAdjacentFlags(int i);
void updateState();
