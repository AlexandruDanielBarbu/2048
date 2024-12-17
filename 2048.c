// librarii necesare
#include "helper.h"

// fisiere externe
#define PATH_saveTablaJoc "./files/tabla_joc"
#define PATH_saveScore "./files/score"
#define PATH_TEXT_howToPlay "./files/how_to_play.txt"
#define PATH_TEXT_credits "./files/credits.txt"
#define PATH_leaderBoard "./files/leaderboards"
#define PATH_TEXT_gameOver "./files/game_over.txt"
#define PATH_TEXT_mainMenu "./files/main_menu.txt"

/*======GAMEPLAY LOOP======*/
#define PANOU_CONTROL_H 5
#define PANOU_JOC_H 20
#define PADDING_LEFT 5
#define PADDING_MIDDLE 10
#define TIMER_SECONDS 30

// organizarea liniilor conform directiei precizate
void organise_rows(int *linie, int dir)
{
    int i, contor;
    switch (dir)
    {
    // 0 sau 3 reprezinta directiile de la dreapta la stanga, respectiv de jos in sus
    case 0:
    case 3:
        contor = 0;
        for (i = 0; i < 4; i++)
        {
            if (linie[i] != 0)
            {
                // pentru fiecare element nenul, verific daca pozitia contorului este libera
                if (linie[contor] == 0)
                {
                    // daca aceasta pozitie este libera, voi muta valoarea din linie[i]
                    linie[contor] = linie[i];
                    linie[i] = 0;
                }
                // odata ocupat, contorul trece la urmatorul spatiu
                contor++;
            }
        }
        break;
    // pentru cazul 1 sau 2 al directiei se face acelasi lucur ca si mai sus, dar invers
    case 2:
    case 1:
        contor = 3;
        for (i = 3; i >= 0; i--)
        {
            if (linie[i] != 0)
            {
                if (linie[contor] == 0)
                {
                    linie[contor] = linie[i];
                    linie[i] = 0;
                }
                contor--;
            }
        }
        break;

    default:
        break;
    }
}

// organizarea coloane conform directiei precizate
void organise_column(int **tablaJoc, int columnToOrganise, int dir)
{
    int i;
    // se copiaza coloana unei matrice intr-un vector
    int *column = (int *)malloc(4 * sizeof(int));
    for (i = 0; i < 4; i++)
    {
        column[i] = tablaJoc[i][columnToOrganise];
    }

    // se organizeaza coloana
    organise_rows(column, dir);

    // se reflecta schimbarile in matricea jocului
    for (i = 0; i < 4; i++)
    {
        tablaJoc[i][columnToOrganise] = column[i];
    }

    // se elibereaza vectorul auxiliar folosit
    free(column);
}

// verific daca mai este loc pe tabla de joc
int eLoc(int **tablaJoc)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (tablaJoc[i][j] == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

// verific daca exista o celula cu numarul 2048 in ea
int isWinner(int **tablaJoc)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (tablaJoc[i][j] == 2048)
            {
                return 1;
            }
        }
    }
    return 0;
}

// generez aleator un 2 sau 4 si il plasez in tabla de joc
void generate2or4(int **tablaJoc)
{

    int number, x, y;

    // generez aleator un 0 sau 1
    number = rand() % 2;
    x = rand() % 4;
    y = rand() % 4;

    // daca este loc pe tabla generez un numar 2 sau 4
    if (eLoc(tablaJoc))
    {
        // cat timp nimeresc pe o pozitie deja ocupata generez alte coordonate pt numarul meu
        while (tablaJoc[y][x] != 0)
        {
            x = rand() % 4;
            y = rand() % 4;
        }

        // 0 inseamna 2, iar 1 inseamna 4
        switch (number)
        {
        case 0:
            number = 2;
            break;

        case 1:
            number = 4;
            break;

        default:
            break;
        }

        // la final se adauga numarul generat in tabla de joc
        tablaJoc[y][x] = number;
    }
}

// prototipul functiei
void AIhandleCommand(int **tablaJoc, int *globalScore);

// functie ce imi numara un anumit nr de secunde si executa la final o alta functie
int Timer(WINDOW *gameInfoWindow, WINDOW *gameWindow, int max_terminal_x, int **tablaJoc, int *globalScore)
{
    int input;
    int nrOfSeconds = TIMER_SECONDS;

    // setez inputul la nodelay() pt a nu bloca codul din executie cu getch()
    nodelay(gameWindow, true);

    // voi porni si mentine cronometrul pornit atata timp cat citesc input invalid
    while (nrOfSeconds > 0)
    {
        // indicator vizual al timerului
        mvwprintw(gameInfoWindow, 4, max_terminal_x / 2 - 3, "Timer: %02d", nrOfSeconds);
        wrefresh(gameInfoWindow);

        // numar cu 60 de secunde in viitor
        clock_t stop = clock() + CLOCKS_PER_SEC;
        // astept 60 de secunde pana sa scand din secunde 1
        while (clock() < stop)
        {
            input = wgetch(gameWindow);
            if (goodInput(input))
            {
                // revin la blocking mode-ul normal al lui getch()
                nodelay(gameWindow, false);
                return input;
            }
        }

        nrOfSeconds--;
    }
    AIhandleCommand(tablaJoc, globalScore);
    
    // functie executata cu succes
    return 0;

}

// prototipul functiei mele detaliate mai jos
int Movement(int **tablaJoc, int dir, int *nrOfCellsCleared);

// codul ce va imbina logica de mai sus cu codul ncurses
// prototipul functiei, deoarece vreau ca din GameLoop sa merg in MainMenu() si invers
void GameLoop(int max_terminal_x, int max_terminal_y, int **tablaJoc, enum GameState state);

void PrintTextFromFileInWindow(char *srcFileName, WINDOW *menuWindow);

void PrintTablaJoc(int **tablaJoc, WINDOW *gameWindow)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            // aici voi face cu switch pt cateva din valori ca sa le afisez cu culori
            switch (tablaJoc[i][j])
            {
            // ce este 0 nu va fi vizibil
            case 0:
                wattron(gameWindow,A_INVIS);
                mvwprintw(gameWindow, 3 + i, 3 + j * 5, "%4d", tablaJoc[i][j]);
                wattroff(gameWindow,A_INVIS);
                break;
            // aici se pot pune culori pentru fiecare din puterile lui 2,
            // nu am implementat culori deoarece terminalul meu nu suporta culori,
            // am incercat sa compensez pentru aceasta pierdere prin alte idei aduse jocului
            default:
                mvwprintw(gameWindow, 3 + i, 3 + j * 5, "%4d", tablaJoc[i][j]);
                break;
            }
        }
    }
    wrefresh(gameWindow);
}

// functie ce afiseaza text la final de runda
void WinLooseText(WINDOW *gameWindow, int max_terminal_x, int score, int **tablaJoc)
{
    PrintTextFromFileInWindow(PATH_TEXT_gameOver,gameWindow);

    mvwprintw(gameWindow, 10, max_terminal_x / 2 - 30, "Your final score was: %d.", score);
    mvwprintw(gameWindow, 11, max_terminal_x / 2 - 30, "Press any key to submit your score!");
    if (isWinner(tablaJoc))
    {
        mvwprintw(gameWindow, 13, max_terminal_x / 2 - 30, "Ai castigat! Felicitari!!! :))");
    }
    else
        mvwprintw(gameWindow, 13, max_terminal_x / 2 - 30, "Nu ai ajuns la finalul jocului, te incurajez sa continui!");

    wrefresh(gameWindow);
    getch();
    clear();
    refresh();
}

// functie ce afiseaza informatiile cerute in gameInfoWindow (panoul de control cum este denumit in tema)
void GameInfoWindowText(WINDOW *gameInfoWindow, int max_terminal_x, int score)
{
    time_t t;
    time(&t);
    mvwprintw(gameInfoWindow, 1, 1, "%s\tScore: %d\tWASD | Q-QUIT | U-UNDO", ctime(&t),score);
    wrefresh(gameInfoWindow);
}

// aceasta functie apeleaza Mivement() si genereaza numarul random 2 sau 4 
int ApplyMove(int **tablaJoc, int dir, int *nrOfCellsCleared)
{
    int scor = Movement(tablaJoc, dir, nrOfCellsCleared);

    // odata ce liniile au fost organizate conform mutarii alese,
    // voi pune pe tabla un nou numar
    generate2or4(tablaJoc);

    // returnez scorul din functia Movement()
    return scor;
}

// voi salva tabla de joc si scorul
void SaveGame(int **tablaJoc, int score){
    
    // deschid cele doua fisiere unde voi salva datele 
    // daca nu exista le voi crea si voi pune in ele ce trebuie
    FILE *saveTablaJoc = fopen(PATH_saveTablaJoc,"w+");
    FILE *saveScore = fopen(PATH_saveScore,"w+");
    
    int i,j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            // pun in saveTablaJoc.txt tabla de joc
            fprintf(saveTablaJoc,"%d ",tablaJoc[i][j]);
        }   
    }

    // pun scorul in saveScore.txt
    fprintf(saveScore,"%d",score);
    
    // inchid fisierele dupa ce am terminat cu ele
    fclose(saveTablaJoc);
    fclose(saveScore);
}

// voi incarca tabala de joc si scorul salvate in fisiere in tabla de joc si scorul folosite de joc
void LoadGame(int **tablaJoc, int *score){
    
    FILE *saveTablaJoc = fopen(PATH_saveTablaJoc,"r");
    FILE *saveScore = fopen(PATH_saveScore,"r");
 
    // ma asigur ca pointerul este la inceputul fisierului
    rewind(saveTablaJoc);
    rewind(saveScore);

    int i,j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            fscanf(saveTablaJoc,"%d",&tablaJoc[i][j]);
        }
    }

    int scorCitit;
    fscanf(saveScore,"%d",&scorCitit);
    *score = scorCitit;
}

void SubmitScore(WINDOW *gameWindow, int max_terminal_x, int score){

    // printez mesajul cu promptul numelui
    mvwprintw(gameWindow,3,PADDING_LEFT, "Introdu numele (3 caractere), apoi apasa orice tasta pt a submita scorul! Atentie: nu gresi");
    mvwprintw(gameWindow,5,PADDING_LEFT+20,"Nume:");
    
    refresh();
    wrefresh(gameWindow);

    int i;
    Player player;
    char *nume = (char*)calloc(4,sizeof(char));

    for (i = 0; i < 3; i++)
    {
        // fac rost de fiecare caracter pt nume    
        nume[i]=toupper(wgetch(gameWindow));
        // il printez in terminal
        mvwprintw(gameWindow,5,PADDING_LEFT+30+i,"%c",nume[i]);
        
        refresh();
        wrefresh(gameWindow);
    }
    
    // astept ca userul sacontinue
    getch();

    // termin numele de formatat
    nume[strlen(nume)] = '\0';
    // pun numele in player.name
    strcpy(player.name,nume);

    // pun scorul in player.score
    player.score = score;

    // deschid fisierul playerii si adaug noul player
    FILE *leaderboard = fopen(PATH_leaderBoard,"ab+");
    fwrite(&player,sizeof(Player),1,leaderboard);
    fclose(leaderboard);

    clear();
    refresh();
}
/*======GAMEPLAY LOOP======*/


/*======MENIUL PRINCIPAL======*/
#define MENU_WINDOW_HEIGHT 50
#define MENU_ITEMS_PADDING 10
#define MENU_NUMBER_OF_OPTIONS 6

// prototipul functiei detaliate mai jos
void MainMenu(int max_terminal_x, int max_terminal_y, int **tablaJoc);
/*======MENIUL PRINCIPAL======*/


/*======LEADERBOARD======*/
#define NR_OF_PLAYERS 5

// la inceput de joc voi initializa Leaderboardul
void InitLeaderboard(){
    FILE *leaderboard = fopen(PATH_leaderBoard,"ab+");
    
    // voi scrie 5 playeri cu nicio data in ei 
    Player playerGol;
    playerGol.score = 0;
    strcpy(playerGol.name,"___");

    fwrite(&playerGol,sizeof(Player),5,leaderboard);
    
    fclose(leaderboard);    
}

void PrintLeaderboard(int height, int width){
    
    // se deschide fisierul
    FILE *leaderboard=fopen(PATH_leaderBoard,"rb");

    int nrOfPlayers = 0;

    // calculez nr de playeri din fisier
    fseek(leaderboard,0,SEEK_END);
    nrOfPlayers = (ftell(leaderboard))/sizeof(Player);    
    
    // ma intorc la inceput de fisier
    rewind(leaderboard);
    
    // aloc playerii
    Player *player = (Player*)calloc(nrOfPlayers+1,sizeof(Player));
    
    // apoi ii citesc
    fread(player,sizeof(Player),nrOfPlayers,leaderboard);
    
    // le ordonez descrescator
    int i,j;
    for (i = 0; i < nrOfPlayers; i++)
    {
        for (j = i; j < nrOfPlayers; j++)
        {
            if (player[i].score < player[j].score)
            {
                Player aux;

                aux = player[i];
                player[i]=player[j];
                player[j]=aux;
            }
        }
    }
    
    // creez fereastra
    WINDOW *leaderBoard = newwin(height,width,0,0);
    box(leaderBoard,0,0);

    // afisez primii 5 playeri cu scorul cel mai mare
    for (i = 0; i < NR_OF_PLAYERS; i++)
    {   
        // printez scorul
        mvwprintw(leaderBoard,i+1,PADDING_LEFT,"%s",player[i].name);
        mvwprintw(leaderBoard,i+1,PADDING_LEFT+3," ");
        mvwprintw(leaderBoard,i+1,PADDING_LEFT+4,"%d",player[i].score);
    }

    refresh();
    wrefresh(leaderBoard);
    
    getch();

    free(player);  
    clear();
    refresh();
}
/*======LEADERBOARD======*/

// creeaza o fereastra cu dimensiunile date si prezinta continutul dintr-un fisier text
#define MAX_MESSAGE_LENGTH 200
void WindowDisplayText(int height, int width, char *srcFileName){
    WINDOW *menuWindow = newwin(height,width,0,0);
    box(menuWindow,0,0);

    PrintTextFromFileInWindow(srcFileName, menuWindow);
    refresh();
    wrefresh(menuWindow);
    
    getch();
    
    clear();
    refresh();
}

void PrintTextFromFileInWindow(char *srcFileName, WINDOW *menuWindow)
{
    FILE *src = fopen(srcFileName, "r");
    int lineNr = 1;
    char line[MAX_MESSAGE_LENGTH];

    rewind(src);
    while (fgets(line, MAX_MESSAGE_LENGTH, src) != NULL)
    {
        mvwprintw(menuWindow, lineNr, PADDING_LEFT, "%s", line);
        lineNr++;
    }
    fclose(src);
}

/*======PAUSE MENU======*/
#define PAUSE_MENU_HEIGHT 5
#define PAUSE_MENU_WIDTH 40
#define PAUSE_MENU_X 3
#define PAUSE_MENU_y 3

// cod similar cu Credits si how to play
void PauseMenuWindow(int y, int x){

    WINDOW *pauseWindow = newwin(PAUSE_MENU_HEIGHT,PAUSE_MENU_WIDTH,y,x);
    // desenez o bordura diferita de cea default
    char topBorder = '~';
    wborder(pauseWindow,0,0,topBorder,0,topBorder,topBorder,0,0);

    mvwprintw(pauseWindow,0,PAUSE_MENU_WIDTH/2-2,"PAUSE");
    mvwprintw(pauseWindow,2,PADDING_LEFT,"Press any key to continue");

    refresh();
    wrefresh(pauseWindow);

    getch();

    clear();
    refresh();
}
/*======PAUSE MENU======*/


// functia main a programului
int main(int argc, char **argv)
{
    // magie ca sa am numere random
    srand(time(NULL));

    // START
    initscr();
    
    int i;
    // alocare memorie pt tabla de joc pe care o voi flosi mereu pe parcursul jocului
    int **tablaJoc = (int **)calloc(4, sizeof(int *));
    for (i = 0; i < 4; i++)
    {
        tablaJoc[i] = (int *)calloc(4, sizeof(int));
    }

    // initializes partida
    initializareJoc();

    // dimensiunile maxime ale terminalului deschis
    int max_terminal_x, max_terminal_y;
    getmaxyx(stdscr, max_terminal_y, max_terminal_x);

    // pornire joc in mod indirect
    MainMenu(max_terminal_x, max_terminal_y,tablaJoc);

    // dealoc memoria pt tabla de joc
    free(tablaJoc);
    endwin();
    // END

    return 0;
}

// Implementarea functiei GameLoop()
void GameLoop(int max_terminal_x, int max_terminal_y, int **tablaJoc, enum GameState state)
{
    int i, j, score=0;
    enum Direction direction = idle;

    if (state == NewGame_State)
    {
        // golesc tabla dac aintru in new game
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                tablaJoc[i][j]=0;
            }
            
        }
        
        // se pun cele doua valori random initiale in tabela
        generate2or4(tablaJoc);
        generate2or4(tablaJoc);
    }
    else{
        // altfel se citesc tabla si scorul din fisierele respective
        LoadGame(tablaJoc,&score);
    }
    

    // game info window
    WINDOW *gameInfoWindow = newwin(PANOU_CONTROL_H, max_terminal_x, 0, 0);
    box(gameInfoWindow, 0, 0);

    // game window
    WINDOW *gameWindow = newwin(PANOU_JOC_H, max_terminal_x, PANOU_CONTROL_H + 1, 0);
    box(gameWindow, 0, 0);

    // dau refresh ca sa apara pe ecran ce trebuie
    refresh();
    wrefresh(gameWindow);
    wrefresh(gameInfoWindow);

    // activez keypadul pentru a inregistra sagetelele
    keypad(gameWindow, true);

    // cat timp jocul ruleaza fac urmatoarele
    while (FOREVER)
    {
        int exitToMainMenu = 0;
        //state = quitState

        // arat timpul curent si restul informatiilor in game info window
        GameInfoWindowText(gameInfoWindow, max_terminal_x, score);

        // printez matricea jocului
        PrintTablaJoc(tablaJoc, gameWindow);

        // fac rost de inputul bun al jucatorului
        int input;
        input = Timer(gameInfoWindow, gameWindow, max_terminal_x, tablaJoc, &score);
        
        int directie = 0;

        // analizez inputul
        switch (input)
        {
        case 'w':
        case 'W':
        case KEY_UP:
            // enum pt directie
            directie = 0;
            direction = up;
            // la fiecare pas corect salvez jocul pt a putea face undo
            SaveGame(tablaJoc,score);
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
            directie = 1;
            direction = right;
            SaveGame(tablaJoc,score);
            break;

        case 's':
        case 'S':
        case KEY_DOWN:
            directie = 2;
            direction = down;
            SaveGame(tablaJoc,score);
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
            directie = 3;
            direction = left;
            SaveGame(tablaJoc,score);
            break;

        case 'q':
        case 'Q':
            // QUIT
            exitToMainMenu = 1;
            break;

        case 'p':
        case 'P':
            // PAUSE
            PauseMenuWindow(15,6);
            
            // dupa ce ies redesenez marginile jocului
            box(gameInfoWindow, 0, 0);
            box(gameWindow,0,0);

            refresh();
            wrefresh(gameWindow);
            wrefresh(gameInfoWindow);

            direction = idle;
            directie = -1;
            break;

        case 'u':
        case 'U':
            // UNDO
            LoadGame(tablaJoc,&score);
            direction = idle;
            directie = -1;
            break;
        
        default:
            directie = -1;
            direction = idle;
            break;
        }

        // testez daca ies la meniul principal
        if (exitToMainMenu)
        {
            SaveGame(tablaJoc,score);
            clear();
            refresh();
            break;
        }
        
        // daca inputul este bun fac urmatoarele
        if (directie != -1)
        {
            // acest int ignore este explcat mai jos
            // in esenta ignore retine cate casute au fost eliberate
            int ignore = 0;
            score += ApplyMove(tablaJoc, directie, &ignore);
            
            werase(gameWindow);
            box(gameWindow, 0, 0);
            wrefresh(gameWindow);

        }

        // testez daca am castigat sau am pierdut
        if (!eLoc(tablaJoc) || isWinner(tablaJoc))
        {
            
            // ma asigur ca matricea nu este printata cand e game over 
            werase(gameWindow);
            box(gameWindow, 0, 0);
            wrefresh(gameWindow);

            WinLooseText(gameWindow, max_terminal_x, score, tablaJoc);
                        
            // submitez scorul
            SubmitScore(gameWindow,max_terminal_x,score);
            
            break;
        }

    }
}

// miscarea facuta de calculator daca expira timpul
void AIhandleCommand(int **tablaJoc, int *globalScore)
{
    int i;
    // fac o cpie la tabla de joc
    int **tablaJocCopy;
    // aloc memorie pentru copie
    tablaJocCopy = (int **)malloc(4 * sizeof(int *));
    for (i = 0; i < 4; i++)
    {
        tablaJocCopy[i] = (int *)malloc(4 * sizeof(int));
    }
    // copiez efectiv tabla in copie
    CPYelement(tablaJoc, tablaJocCopy);

    // aici am cele 4 miscari posibile
    AImove move0, move1, move2, move3;

    // acest ignore retine numarul de celule curatate, dar in mod sugestiv numelui nu l-am utilizat in aceasta abordare
    int ignore = 0;

    // pentru fiecare comanda posibila voi apela Movement()
    move0.score = Movement(tablaJocCopy, 0, &ignore);
    move0.dir = 0;
    // refac copia tablei de joc
    CPYelement(tablaJoc, tablaJocCopy);

    move1.score = Movement(tablaJocCopy, 1, &ignore);
    move1.dir = 1;
    CPYelement(tablaJoc, tablaJocCopy);

    move2.score = Movement(tablaJocCopy, 2, &ignore);
    move2.dir = 2;
    CPYelement(tablaJoc, tablaJocCopy);

    move3.score = Movement(tablaJocCopy, 3, &ignore);
    move3.dir = 3;
    CPYelement(tablaJoc, tablaJocCopy);

    // compar scorul maxim de la fiecare si il retin
    AImove computerMove = maxim(move0, maxim(move1, maxim(move2, move3)));

    // comanda ce rezulta in cel mai mare scor se aplica
    ApplyMove(tablaJoc, computerMove.dir, &ignore);
    (*globalScore) += computerMove.score;
}

// se modifica tabla jocului conform miscarii alese
int Movement(int **tablaJoc, int dir, int *nrOfCellsCleared)
{
    int i, j;
    *nrOfCellsCleared = 0;
    
    // acest scor este calculat pentru aceasta mutare,
    // ulterior este adunat la scorul global
    int score = 0;

    //  mut fiecare linie cum trebuie in directia indicata
    switch (dir)
    {
    case 0:
        for (j = 0; j < 4; j++)
        {
            // organizez fiecare coloana mai intai
            organise_column(tablaJoc, j, dir);

            for (i = 0; i < 4; i++)
            {
                if (i != 0)
                {
                    // odata organizata o linie voi uni patratelele cu aceleasi valori
                    // din directia dorita spre cea opusa
                    if (tablaJoc[i][j] == tablaJoc[i - 1][j])
                    {
                        tablaJoc[i - 1][j] *= 2;
                        score += tablaJoc[i - 1][j];
                        (*nrOfCellsCleared)++;
                        tablaJoc[i][j] = 0;

                        // dupa ce unesc doua patrate voi reorganiza linia
                        organise_column(tablaJoc, j, dir);
                    }
                }
            }
        }
        break;

    case 3:
        for (i = 0; i < 4; i++)
        {
            organise_rows(tablaJoc[i], dir);

            for (j = 0; j < 4; j++)
            {
                if (j != 0)
                {
                    // odata organizata o linie voi uni patratelele cu aceleasi valori
                    // din directia dorita spre cea opusa
                    if (tablaJoc[i][j] == tablaJoc[i][j - 1])
                    {
                        tablaJoc[i][j - 1] *= 2;
                        score += tablaJoc[i][j - 1];
                        (*nrOfCellsCleared)++;
                        tablaJoc[i][j] = 0;

                        // dupa ce unesc doua patrate voi reorganiza linia
                        organise_rows(tablaJoc[i], dir);
                    }
                }
            }
        }
        break;

    case 2:
        for (j = 0; j < 4; j++)
        {
            organise_column(tablaJoc, j, dir);

            for (i = 3; i >= 0; i--)
            {
                if (i != 3)
                {
                    // odata organizata o linie voi uni patratelele cu aceleasi valori
                    // din directia dorita spre cea opusa
                    if (tablaJoc[i][j] == tablaJoc[i + 1][j])
                    {
                        tablaJoc[i + 1][j] *= 2;
                        score += tablaJoc[i + 1][j];
                        (*nrOfCellsCleared)++;
                        tablaJoc[i][j] = 0;

                        // dupa ce unesc doua patrate voi reorganiza linia
                        organise_column(tablaJoc, j, dir);
                    }
                }
            }
        }
        break;

    case 1:
        for (i = 0; i < 4; i++)
        {
            organise_rows(tablaJoc[i], dir);

            for (j = 3; j >= 0; j--)
            {
                if (j != 3)
                {
                    // odata organizata o linie voi uni patratelele cu aceleasi valori
                    // din directia dorita spre cea opusa
                    if (tablaJoc[i][j] == tablaJoc[i][j + 1])
                    {
                        tablaJoc[i][j + 1] *= 2;
                        score += tablaJoc[i][j + 1];
                        (*nrOfCellsCleared)++;
                        tablaJoc[i][j] = 0;

                        // dupa ce unesc doua patrate voi reorganiza linia
                        organise_rows(tablaJoc[i], dir);
                    }
                }
            }
        }
        break;

    default:
        break;
    }

    return score;
}

void MainMenu(int max_terminal_x, int max_terminal_y, int **tablaJoc)
{
    enum GameState stateOfPlay = Idle_State;
    // crearea ferestrei meniu
    WINDOW *menuWindow = newwin(MENU_WINDOW_HEIGHT, max_terminal_x, 0, 0);
    box(menuWindow, 0, 0);

    // afisarea ferestrei meniu
    refresh();
    wrefresh(menuWindow);

    // pt a recunoaste sagetile
    keypad(menuWindow, true);

    // creez lista de optiuni din meniu
    char *choices[MENU_NUMBER_OF_OPTIONS] = {
                                            "NEW GAME", 
                                            "RESUME", 
                                            "HOW TO PLAY", 
                                            "CREDITS", 
                                            "LEADERBOARD", 
                                            "QUIT"
                                            };

    int select;
    int highlight = 0;

    PrintTextFromFileInWindow(PATH_TEXT_mainMenu,menuWindow);

    while (FOREVER)
    {
        // printez cele MENU_NUMBER_OF_OPTIONS optiuni ale meniului
        int i;
        for (i = 0; i < MENU_NUMBER_OF_OPTIONS; i++)
        {
            if (i == highlight)
                wattron(menuWindow, A_REVERSE);

            mvwprintw(menuWindow, i + 10, MENU_ITEMS_PADDING, "%s", choices[i]);
            wattroff(menuWindow, A_REVERSE);
        }

        // verific ce am apasat ca sa vad ce fac 
        select = wgetch(menuWindow);

        switch (select)
        {
        case 'W':
        case 'w':
        case 'A':
        case 'a':
        case KEY_LEFT:
        case KEY_UP:
            highlight--;
            if (highlight < 0)
            {
                highlight = 0;
            }
            break;

        case 'S':
        case 's':
        case 'D':
        case 'd':
        case KEY_RIGHT:
        case KEY_DOWN:
            highlight++;
            if (highlight > MENU_NUMBER_OF_OPTIONS - 1)
            {
                highlight = MENU_NUMBER_OF_OPTIONS - 1;
            }
            break;

        default:
            break;
        }

        // daca am apasat tasta enter
        int quitAction = 0;
        if (select == 10)
        {
            switch (highlight)
            {
            case 0:
                // NEW GAME
                clear();
                refresh();
                
                stateOfPlay = NewGame_State;
                GameLoop(max_terminal_x, max_terminal_y,tablaJoc,stateOfPlay);
                break;

            case 1:
                // RESUME
                FILE *saveScore = fopen(PATH_saveScore,"r");
                FILE *saveTablaJoc = fopen(PATH_saveTablaJoc,"r");

                if (saveScore!= NULL && saveTablaJoc!= NULL)
                {
                    fclose(saveScore);
                    fclose(saveTablaJoc);
                    
                    clear();
                    refresh();
                    
                    stateOfPlay = ResumeGame_State;
                    GameLoop(max_terminal_x, max_terminal_y,tablaJoc,stateOfPlay);
                }
                break;

            case 2:
                // HOW TO PLAY
                WindowDisplayText(max_terminal_y,max_terminal_x,PATH_TEXT_howToPlay);
                //howToPlay(MENU_WINDOW_HEIGHT,max_terminal_x /* char srcFileName*/);
                break;

            case 3:
                // CREDITS
                WindowDisplayText(max_terminal_y,max_terminal_x,PATH_TEXT_credits);
                break;
            
            case 4:
                // LEADERBOARDS
                FILE *leaderboard = fopen(PATH_leaderBoard,"rb");
                if (leaderboard != NULL){
                    fclose(leaderboard);
                    
                    InitLeaderboard();
                    PrintLeaderboard(max_terminal_y,max_terminal_x);
                }
                break;

            case 5:
                // QUIT GAME
                stateOfPlay = QuitGame_State;
                break;

            default:
                break;
            }
        }
        else if (tolower(select) == 'q')
        {
            stateOfPlay = QuitGame_State;
        }

        if (stateOfPlay == QuitGame_State)
        {
            stateOfPlay = Idle_State;
            break;
        }

        // ma asigur ca borderul ramane
        box(menuWindow, 0, 0);
        refresh();
        wrefresh(menuWindow);
    }
}