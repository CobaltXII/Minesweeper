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

// A Minesweeper game.
class Minesweeper {
public:
	// The graphics adapter.
	Graphics adapter;

	// The game board.
	Cell* board;

	// The sprites.
	Sprite border[9];
	Sprite tile[16];
	Sprite counter[10];
	Sprite smiley[5];
	Sprite frame;

	// The game's settings.
	int x_cells;
	int y_cells;
	int mines;

	// The game's state.
	enum {
		GAME_WAITING,
		GAME_PLAYING,
		GAME_WINNER,
		GAME_LOSER
	} state;
	Uint32 start_ticks = 0;
	Uint32 end_ticks = 0;
	int flags;

	// The game board's render offset.
	const int xoff = 10;
	const int yoff = 50;

	// Default constructor.
	Minesweeper(int x_cells, int y_cells, int mines) {
		// Create the graphics adapter.
		adapter = Graphics("Minesweeper", x_cells * 16 + xoff + 10, y_cells * 16 + yoff + 10, 2);
		// Load the sprites.
		load_borders();
		load_tiles();
		load_counters();
		load_smileys();
		load_frame();
		// Allocate the game board.
		this->x_cells = x_cells;
		this->y_cells = y_cells;
		this->mines = mines;
		board = new Cell[x_cells * y_cells];
		// Generate the game board.
		generate_board();
	}
};