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

void initBoard(enum difficulty diff, Board *b);
void freeBoard(Board *b);
void initRecs(Board *b);
void revealTile(int i, Board *b);
void flagTile(int i, Board *b);
int getAdjacentTile(int i, enum direction dir, Board *b);
int getAdjacentMines(int i, Board *b);
int getAdjacentFlags(int i, Board *b);
void updateState(Board *b);
