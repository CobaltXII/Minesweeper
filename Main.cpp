#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>

#include <SDL.h>

#include "Sprite.hpp"
#include "Graphics.hpp"
#include "Minesweeper.hpp"

// Print usage information and exit.
void usage(char** argv) {
	fprintf(stderr, "Usage: %s [<-b|-i|-e>|<W> <H> <M>]\n", argv[0]);
	fprintf(stderr, "\t-b          Beginner mode (9x9 with 10 mines)\n");
	fprintf(stderr, "\t-i          Intermediate mode (16x16 with 40 mines)\n");
	fprintf(stderr, "\t-e          Expert mode (30x16 with 99 mines)\n");
	fprintf(stderr, "\t<W> <H> <M> Custom mode (WxH with M mines)\n");
	exit(EXIT_FAILURE);
}

// Entry point.
int main(int argc, char** argv) {
	// Parse the command line arguments.
	int w;
	int h;
	int mines;
	if (argc != 1 && argc != 2 && argc != 4) {
		usage(argv);
	} else if (argc == 1) {
		// Play in intermediate mode by default.
		w = 16;
		h = 16;
		mines = 40;
	} else if (argc == 2) {
		if (std::string(argv[1]) == "-b") {
			w = 9;
			h = 9;
			mines = 10;
		} else if (std::string(argv[1]) == "-i") {
			w = 16;
			h = 16;
			mines = 40;
		} else if (std::string(argv[1]) == "-e") {
			w = 30;
			h = 16;
			mines = 99;
		} else {
			usage(argv);
		}
	} else if (argc == 4) {
		w = std::stoi(std::string(argv[1]));
		h = std::stoi(std::string(argv[2]));
		mines = std::stoi(std::string(argv[3]));
	}

	// Make sure the mine count is not insane.
	if (mines > w * h - 10) {
		mines = w * h - 10;
	}

	// Create a game.
	Minesweeper minesweeper = Minesweeper(w, h, mines);

	// Start and end the game.
	minesweeper.start();
	minesweeper.end();

	// Exit successfully.
	exit(EXIT_SUCCESS);
}