// librarii necesare
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*======STRUCTS======*/
typedef struct
{
    int score;
    int dir;
} AImove;

typedef struct {
    char name[5];
    int score;   
}Player;
/*======STRUCTS======*/

/*======ENUMS======*/
typedef enum GameState{Idle_State, ResumeGame_State, NewGame_State, QuitGame_State};
typedef enum Direction{up, right, down, left, idle};
/*======ENUMS======*/

/*======NCURSES======*/
// constante necesare programului mare
#define FOREVER 1

// aceasta functie apeleaza alte functii specifice ncurses, pentru un gameplay placut :)
void initializareJoc()
{
    // golesc ce e pe ecran just in case
    clear();
    // dau refresh sa ma asigur ca incep fresh
    refresh();
    // opresc cursorul pt ca nu am nevoie sa il vad
    curs_set(0);
    // nu printez tastele pe care apas
    noecho();
    // trimit inputul direct la aplicatie (daca am inteles bine, inteleg ca e mai rapid inputul asa)
    cbreak();
}
/*======NCURSES======*/

// aceasta functie copiaza toate elementele matricei de joc in alta matrice
void CPYelement(int **matrice_sursa, int **matrice_destinatie)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            matrice_destinatie[i][j] = matrice_sursa[i][j];
        }
    }
}

/*  Explicatie maxim
    calculatorul va putea muta singur dupa un nr de secunde,
    fiecare mutare posibila are un scor si o directie,
    aceasta functie returneaza miscarea cu cel mai mare scor, astfel pastrand si directia 
*/
AImove maxim(AImove a, AImove b)
{
    if (a.score > b.score)
    {
        return a;
    }
    else
        return b;
}


// verific daca inputul meu este bun
int goodInput(int input)
{
    switch (input)
    {
    // controale miscari joc
    case 'w':
    case 'W':
    case KEY_UP:
    case 'd':
    case 'D':
    case KEY_RIGHT:
    case 's':
    case 'S':
    case KEY_DOWN:
    case 'a':
    case 'A':
    case KEY_LEFT:
    case 'q':
    case 'Q':
    
    // pause
    case 'p':
    case 'P':
    
    // undo
    case 'u':
    case 'U':
        return 1;
        break;
    
    default:
        break;
    }
    return 0;
}
