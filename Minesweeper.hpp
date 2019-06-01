// Border tile constants.
enum {
	BORDER_TOP_LEFT,
	BORDER_TOP_RIGHT,
	BORDER_BOTTOM_LEFT,
	BORDER_BOTTOM_RIGHT,
	BORDER_X_SOLID,
	BORDER_Y_SOLID,
	BORDER_JOINT_LEFT,
	BORDER_JOINT_RIGHT,
	BORDER_SOLID
};

// Tile constants.
enum {
	TILE_COVERED,
	TILE_UNCOVERED,
	TILE_FLAGGED,
	TILE_UNUSED1,
	TILE_UNUSED2,
	TILE_MINE1,
	TILE_MINE2,
	TILE_MINE3
};

// Smiley constants.
enum {
	SMILEY_DEFAULT,
	SMILEY_PRESSED,
	SMILEY_WORRIED,
	SMILEY_HAPPY,
	SMILEY_SAD
};

// A cell.
struct Cell {
	int neighbours;
	bool is_uncovered;
	bool is_flagged;
	bool is_mine;
	bool is_culprit;
};
