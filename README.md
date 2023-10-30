# Chess Project

This project is an implementation of a chess game in C with an OpenGL interface. It provides a user-friendly interface for players to engage in classic chess matches.

## Features

- Simplified user interface using OpenGL
- Piece movements following standard chess rules
- Check and checkmate detection
- Turn-based gameplay for white and black players

## Requirements

- GCC compiler
- OpenGL libraries

On Debian-based systems (like Ubuntu), you can install the required packages using the following commands:

```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
sudo apt-get install libsoil-dev
```

## Compilation and Execution

To compile the project, use the following command:
```bash
make
```

To run the program, use the following command:
```bash
./ChessAI
```

## Source
I mainly use this website to implement the bitboards https://www.chessprogramming.org/Efficient_Generation_of_Sliding_Piece_Attacks
