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

	// Load the border sprites.
	void load_borders() {
		Sprite sheet("Border.png");
		for (int i = 0; i < 9; i++) {
			border[i] = Sprite(sheet, i * 10, 0, 10, 10);
		}
	}

	// Load the tile sprites.
	void load_tiles() {
		Sprite sheet("Tile.png");
		for (int i = 0; i < 8; i++) {
			tile[i] = Sprite(sheet, i * 16, 0, 16, 16);
		}
		for (int i = 0; i < 8; i++) {
			tile[i + 8] = Sprite(sheet, i * 16, 16, 16, 16);
		}
	}

	// Load the counter sprites.
	void load_counters() {
		Sprite sheet("Counter.png");
		for (int i = 0; i < 10; i++) {
			counter[i] = Sprite(sheet, i * 13, 0, 13, 23);
		}
	}

	// Load the smiley sprites.
	void load_smileys() {
		Sprite sheet("Smiley.png");
		for (int i = 0; i < 5; i++) {
			smiley[i] = Sprite(sheet, i * 26, 0, 26, 26);
		}
	}

	// Load the frame sprite.
	void load_frame() {
		frame = Sprite("Frame.png");
	}

	// Check if a coordinate is within the bounds of the game board.
	inline bool is_bound(int x, int y) {
		return x >= 0 && x < x_cells &&
			   y >= 0 && y < y_cells;
	}

	// Generate the game board.
	void generate_board() {
		flags = 0;
		// Clear the game board.
		for (int y = 0; y < y_cells; y++) {
			for (int x = 0; x < x_cells; x++) {
				Cell& cell = board[y * x_cells + x];
				cell.neighbours = 0;
				cell.is_uncovered = false;
				cell.is_flagged = false;
				cell.is_mine = false;
				cell.is_culprit = false;
			}
		}
		// Add some mines.
		for (int i = 0; i < mines; i++) {
			while (1) {
				int x = rand() % x_cells;
				int y = rand() % y_cells;
				Cell& cell = board[y * x_cells + x];
				if (!cell.is_mine) {
					cell.is_mine = true;
					break;
				}
			}
		}
		// Calculate the neighbouring mine count of each cell.
		calculate_neighbours();
	}

	// Calculate the neighbouring mine count of each cell.
	void calculate_neighbours() {
		for (int y = 0; y < y_cells; y++) {
			for (int x = 0; x < x_cells; x++) {
				Cell& cell = board[y * x_cells + x];
				cell.neighbours = 0;
				for (int v = 0; v < 3; v++) {
					for (int u = 0; u < 3; u++) {
						int i = x - 1 + u;
						int j = y - 1 + v;
						if (is_bound(i, j)) {
							if (board[j * x_cells + i].is_mine) {
								cell.neighbours++;
							}
						}
					}
				}
			}
		}
	}
};