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

	// Divert a cell and it's neighbours so that there are no mines in the
	// neighbourhood of a cell.
	void divert(int x, int y) {
		for (int v = 0; v < 3; v++) {
			for (int u = 0; u < 3; u++) {
				// The variables i and j are the coordinates of a neighbour.
				int i = x - 1 + u;
				int j = y - 1 + v;
				if (is_bound(i, j)) {
					Cell& cell = board[j * x_cells + i];
					if (cell.is_mine) {
						// Divert this mine.
						while (1) {
							// Pick a random position for the mine.
							int n = rand() % x_cells;
							int m = rand() % y_cells;
							if (is_bound(n, m)) {
								Cell& cell = board[m * x_cells + n];
								// Check if the cell at the new position is
								// already a mine.
								if (cell.is_mine) {
									continue;
								}
								// Check if the new position lies within the
								// neighbourhood of the cell to divert away from.
								if (n >= x - 1 && n <= x + 1 &&
									m >= y - 1 && m <= y + 1)
								{
									continue;
								}
								cell.is_mine = true;
								break;
							}
						}
						cell.is_mine = false;
					}
				}
			}
		}
		// Calculate the neighbouring mine count of each cell.
		calculate_neighbours();
	}

	// Check if all non-mine cells have been uncovered.
	bool winner() {
		for (int y = 0; y < y_cells; y++) {
			for (int x = 0; x < x_cells; x++) {
				Cell& cell = board[y * x_cells + x];
				if (!cell.is_mine && !cell.is_uncovered) {
					return false;
				}
			}
		}
		return true;
	}

	// Uncover a cell.
	void uncover(int x, int y) {
		if (state == GAME_WINNER ||
			state == GAME_LOSER) {
			// Can't interact with a board after the game is finished.
			return;
		}
		if (!is_bound(x, y)) {
			return;
		}
		Cell& cell = board[y * x_cells + x];
		if (!cell.is_uncovered) {
			// A cell is being uncovered.
			if (state == GAME_WAITING) {
				state = GAME_PLAYING;
				start_ticks = SDL_GetTicks();
				// Divert mines away from the first click.
				divert(x, y);
			}
			cell.is_uncovered = true;
			if (cell.is_mine) {
				// The player uncovered a mine!
				state = GAME_LOSER;
				end_ticks = SDL_GetTicks();
				cell.is_culprit = true;
			} else if (cell.neighbours == 0) {
				// Recursively uncover neighbouring cells.
				uncover(x - 1, y    );
				uncover(x + 1, y    );
				uncover(x    , y - 1);
				uncover(x    , y + 1);
				uncover(x - 1, y - 1);
				uncover(x - 1, y + 1);
				uncover(x + 1, y - 1);
				uncover(x + 1, y + 1);
			}
		}
	}

	// Flag or unflag a cell.
	void flag(int x, int y) {
		if (state == GAME_WINNER ||
			state == GAME_LOSER) {
			// Can't interact with a board after the game is finished.
			return;
		}
		if (!is_bound(x, y)) {
			return;
		}
		Cell& cell = board[y * x_cells + x];
		if (!cell.is_uncovered) {
			if (state == GAME_WAITING) {
				state = GAME_PLAYING;
				start_ticks = SDL_GetTicks();
			}
			if (cell.is_flagged) {
				flags--;
			} else {
				flags++;
			}
			cell.is_flagged = !cell.is_flagged;
		}
	}
};