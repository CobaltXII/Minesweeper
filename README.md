## Synopsis
A Microsoft Minesweeper clone written in C++11

![Alt text](https://github.com/CobaltXII/Minesweeper/blob/master/Minesweeper.png?raw=true)

## Abstract
Minesweeper is a Microsoft Minesweeper clone written in C++11. From Wikipedia: "Minesweeper is a single-player puzzle video game. The objective of the game is to clear a rectangular board containing hidden "mines" or bombs without detonating any of them, with help from clues about the number of neighboring mines in each field.".

## Dependencies
Minesweeper requires stb_image (Sean T. Barrett), SDL2 and C++11. The dependency stb_image is included in this repository.

## Building
```
./build.sh
```

## Usage
```
cobalt$ ./Minesweeper.o --help
Usage: ./Minesweeper.o [<-b|-i|-e>|<W> <H> <M>]
	-b          Beginner mode (9x9 with 10 mines)
	-i          Intermediate mode (16x16 with 40 mines)
	-e          Expert mode (30x16 with 99 mines)
	<W> <H> <M> Custom mode (WxH with M mines)
```

## Credits
Thanks to Black Squirrel and Emmett N. for ripping the original sprites. Thanks to Microsoft for creating the sprites.

## License
This repository and it's code contents are licensed under the MIT License. All of the sprites in this repository are property of their respective owners (probably Microsoft).