# 2048 - C Game

Barbu Alexandru Daniel


# Table of Contents:

0. [Application Description](#application-description)
1. [Installation and Execution](#installation-and-execution)
2. [Controls](#controls)
3. [Feature Details](#feature-details)
4. [Future Plans](#future-plans)
5. [Personal Impressions](#personal-impressions)


# Application Description

2048 is a game many of you are likely familiar with. This project was developed
as part of `Assignment 2` for the `Computer Programming course` and involves
implementing the `2048 game` in `C`, using the `ncurses library` for the UI.

The application includes many standard game features, which will be detailed
below and explained within the code.


# Installation and Execution

## Steps:

1. Extract the files from the archive.
2. Compile the code using the command: `make`
3. Run the generated executable (the actual game) using the command: `make run`

> **Note:** Compilation is mandatory before running.

## Required libraries:
- ncurses.h,
- time.h,
- stdlib.h,
- ctype.h,
- string.h.


# Controls

> **Note:** Controls can be found in the main menu under `HOW TO PLAY` section.

`WASD` / `Arrow Keys` - Navigate menu options or move numbers during the game.

- `P` - Pause the game.
- `Q` - Back to previous window, or quit the application from the main menu.
- `U` - Undo the last move during the game.
- `CTRL + C` - Force quit the program.


# Feature Details

## Task 1:

The game includes three essential options with their implementations:

- `New Game` - Starts a new game.
- `Resume` - Resumes the last game from memory (state and score are preserved).
- `Quit` - Exits the application.

### Extra:

A constant defines the number of menu items, making it easy to add more tabs
like `HOW TO PLAY`, `CREDITS` and `LEADERBOARD`.

> **Note:** The leaderboard initially has 5 empty slots.

- `CREDITS` provides developer information.
- `HOW TO PLAY` explains the controls.

## Task 2:

> **Note:** The initial score is 0.

Current date and time are displayed and updated after each move.

Short legend of accepted keys is shown without cluttering the screen.

Values of 2 or 4 are generated automatically at game start and after each player move.

### Extra:

The game screen is split into two windows:

- `top window` shows game information.
- `larger window` shows the game board.

> **Note:** A timer above the game board automatically makes a move after 30 seconds.

## Task 3:

The game supports moving and merging tiles, generating new values (2 or 4) on free tiles, and waiting for valid player input.

Merging tiles increases the score based on the value of the new tile.

### Extra:

A `pause menu` allows players time to strategize without obstructing the game board.

The game uses a switch statement for color-coded elements, though my terminal doesn’t support colors.

An UNDO feature is implemented, activated by pressing `U`.

## Task 4:

The AI makes a move `after 30 seconds` if the player is idle.

> **Note:** The AI prioritizes moves that yield the highest score.

### Extra:

Although the assignment required clearing as many tiles as possible, I implemented the AI to maximize the score, considering it more competitive.

## Task 5:

Two functions check if there are available moves or if 2048 is reached.

An appropriate message is displayed at the game's end.

### Extra:

After the game ends, players can submit their names for the leaderboard.


# Future Plans

The game’s solid foundation allows for future enhancements:

- `Modular Board Size` - Support for board sizes of `4x4`, `8x8`, `16x16`, etc.
- `Undo States` - Allow multiple undo actions by saving multiple game states.
- `Settings Panel` - Adjust board size, AI timer duration, and other settings.
- `Difficulty Levels` - Introduce chances of generating values higher than 4 based on difficulty.


# Personal Impressions

I enjoyed working on this project. Initially, I panicked, thinking I wouldn’t finish (I started on January 2, in the evening). Solving problems on my own, using course knowledge and online resources, was very satisfying.

I'm most proud of the `Movement()` function, which is central to the game. I also enjoyed creating the `Timer()` function, which helped me learn how to implement a timer in C. Figuring out how to read a keyboard input without blocking the entire program with `getch()` was particularly challenging.

Although I regret not using macros or function pointers for more generic, dependency-free code, I believe the project is still solid for a first attempt.

I also appreciated using a lightweight library like ncurses. Initially, I feared we would use OpenGL, which I heard is difficult, but ncurses turned out to be very suitable for this small project.