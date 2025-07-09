# C4 (I should think of a better name)

## Overview

C4 is a connect four game solver. Currently it supports these features:

- **Evaluate the score** of any given board (game state).
- **Find the best move** to make from a game state.
- **Self-train** to speed up the calculating speed.
- **Play a game** against the user.

# Build instructions:

## Prerequisites:

- clang++
- cmake
- ninja

## Building:

### Using project env:

Windows:
```
$ . .\projenv.ps1
$ build
```

Linux:
```
$ source projenv
$ build
```

### Using cmake:

```
cd c4
mkdir build
cmake -B build -S .
cmake --build build --target all
```

## Launch instructions:

**Remember to launch c4 in the project root directory**: The default paths for the opening book and the warmup book are under `data/`

Windows:
```
.\build\bin\c4.exe <ARGS>
```

Linux:
```
./build/c4 <ARGS>
```

**Arguments:**

- **Find mode: -f, --find**: The user inputs a sequence representing a connect four board and the program returns the best move to make for that game state. It prints out the best move along with some other information.

- **Play mode: -p, --play**: Start a game against the solver, the player could choose to be either red or yellow.

- **Bot versus bot: -b, --botgame**: Create 2 bots and let them play against each other. In each move, you could see the board, the move they make and the time taken for that move.

- **Training mode: -tr, --training**: This mode lets the solver AI train itself. Basically it creates a game between 2 bots and occasionally randomize the moves to mimic a realistic gameplay scenario. Then it filters out the moves that take longer than 2 seconds to calculate, and contribute back to the warmup book. The warmup book is a type of database, it works the same as the opening book, but is smaller and only contains moves from this training mode. This way the hard moves are persistently saved and provide O(1) lookups.

The program requires the opening book to calculate moves in the early game. The warmup book is optional. By default the books are saved in `data/`, and you **MUST RUN** the c4 executable from the project root directory. If you run the executable from anywhere else, or you have your own books to use, specify the path to the book by the arguments `--opening-book` and `--warmup-book`. For example:
```
./c4 -f --opening-book <path> --warmup-book <path> # Specify both books
./c4 -f --opening-book <path> # Specify one book
./c4 -f <path> <path> # Opening book first, warmup book second
```