#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* #region Structs */
typedef struct AImove {
    int score;
    int dir;
} AImove;

typedef struct Player {
    char name[5];
    int score;
} Player;
/*#endregion*/

/* #region Enums */
typedef enum GameState {
    Idle_State,
    ResumeGame_State,
    NewGame_State,
    QuitGame_State
} GameState;

typedef enum Direction {
    up,
    right,
    down,
    left,
    idle
} Direction;
/*#endregion*/

/* #region Function prototypes */
void initializareJoc();
void CPYelement(int **matrice_sursa, int **matrice_destinatie);
AImove maxim(AImove a, AImove b);
int goodInput(int input);


int eLoc(int **tablaJoc);
int isWinner(int **tablaJoc);


void AIhandleCommand(int **tablaJoc, int *globalScore);
int Movement(int **tablaJoc, int dir, int *nrOfCellsCleared);
void GameLoop(int max_terminal_x, int max_terminal_y, int **tablaJoc, enum GameState state);
void PrintTextFromFileInWindow(char *srcFileName, WINDOW *menuWindow);
void MainMenu(int max_terminal_x, int max_terminal_y, int **tablaJoc);


/*#endregion*/

#endif // GAME_H