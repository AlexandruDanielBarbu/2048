#include "game.h"

void initializareJoc()
{
    // clear screen
    clear();

    // refresh
    refresh();

    // turn cursor off
    curs_set(0);

    // don't print keys
    noecho();

    // redirect input to app for faster processing
    cbreak();
}

void CPYelement(int **matrice_sursa, int **matrice_destinatie)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            matrice_destinatie[i][j] = matrice_sursa[i][j];
}

/*  @brief Decide which move has the best score
 */
AImove maxim(AImove a, AImove b)
{
    if (a.score > b.score)
        return a;
    else
        return b;
}

int goodInput(int input)
{
    switch (input)
    {
    // game input
    case 'w': case 'W': case KEY_UP:
    case 'd': case 'D': case KEY_RIGHT:
    case 's': case 'S': case KEY_DOWN:
    case 'a': case 'A': case KEY_LEFT:
    case 'q': case 'Q':
    // pause
    case 'p': case 'P':
    // undo
    case 'u': case 'U':
        return 1;
        break;

    default:
        break;
    }
    return 0;
}

/* @brief Check if there is space on the board
 */
int eLoc(int **tablaJoc)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (tablaJoc[i][j] == 0)
                return 1;
    return 0;
}

/* @brief Check if 2048 exists on the board
 */
int isWinner(int **tablaJoc)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (tablaJoc[i][j] == 2048)
                return 1;
    return 0;
}
