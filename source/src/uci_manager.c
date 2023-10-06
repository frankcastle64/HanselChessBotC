#include "uci_manager.h"
#include "stdio.h"
#include "string.h"
#include "position.h"
#include "fen_manager.h"
#include "consts.h"
#include "consts.h"
#include "search.h"
#include "display.h"
#include "move_manager.h"
#include "init.h"

#define INPUTBUFFER 400 * 6

// position startpos
// ... moves e2e4 e7e5 b7b8q
// code from vice.
void ParsePosition(char *lineIn, BoardState *boardState)
{

    lineIn += 9;
    char *ptrChar = lineIn;

    if (strncmp(lineIn, "startpos", 8) == 0)
    {
        SetBoard(START_POSITION, boardState);
    }
    else
    {
        ptrChar = strstr(lineIn, "fen");
        if (ptrChar == NULL)
        {
            SetBoard(START_POSITION, boardState);
        }
        else
        {
            ptrChar += 4;
            SetBoard(ptrChar, boardState);
        }
    }

    ptrChar = strstr(lineIn, "moves");

    if (ptrChar != NULL)
    {
        ptrChar += 6;
        while (*ptrChar)
        {
            MakeStrMove(ptrChar, boardState->position);

            while (*ptrChar && *ptrChar != ' ')
                ptrChar++;
            ptrChar++;
        }
    }

    // PrintBoard(boardState);
}
// go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000 movestogo 40
// code from vice.
void ParseGo(char *line, BoardState *boardState)
{

    int depth = -1, movestogo = 30, movetime = -1;
    int time = -1, inc = 0;
    char *ptr = NULL;
    // info->timeset = FALSE;

    if ((ptr = strstr(line, "infinite")))
    {
        ;
    }

    if ((ptr = strstr(line, "binc")) && boardState->position->sideToMove == COLOR_BLACK)
    {
        inc = atoi(ptr + 5);
    }

    if ((ptr = strstr(line, "winc")) && boardState->position->sideToMove == COLOR_WHITE)
    {
        inc = atoi(ptr + 5);
    }

    if ((ptr = strstr(line, "wtime")) && boardState->position->sideToMove == COLOR_WHITE)
    {
        time = atoi(ptr + 6);
    }

    if ((ptr = strstr(line, "btime")) && boardState->position->sideToMove == COLOR_BLACK)
    {
        time = atoi(ptr + 6);
    }

    if ((ptr = strstr(line, "movestogo")))
    {
        movestogo = atoi(ptr + 10);
    }

    if ((ptr = strstr(line, "movetime")))
    {
        movetime = atoi(ptr + 9);
    }

    if ((ptr = strstr(line, "depth")))
    {
        depth = atoi(ptr + 6);
    }

    if (movetime != -1)
    {
        time = movetime;
        movestogo = 1;
    }

    // info->starttime = GetTimeMs();
    // info->depth = depth;

    // if(time != -1) {
    // 	info->timeset = TRUE;
    // 	time /= movestogo;
    // 	time -= 50;
    // 	info->stoptime = info->starttime + time + inc;
    // }

    // if (depth == -1)
    // {
    //     info->depth = MAX_DEPTH;
    // }

    Think(boardState->position, 8);
}

void UciLoop()
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char line[INPUTBUFFER];
    BoardState boardState[1];
    printf("id name %s\n", "Hansel v1.0");
    printf("id author Gurudas Bhandarkar\n");
    printf("uciok\n");

    while (1)
    {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, INPUTBUFFER, stdin))
            continue;

        if (line[0] == '\n')
            continue;

        else if (!strncmp(line, "isready", 7))
        {
            printf("readyok\n");
            continue;
        }
        else if (!strncmp(line, "position", 8))
        {
            InitAll(boardState);
            ParsePosition(line, boardState);
        }
        else if (!strncmp(line, "ucinewgame", 10))
        {
            InitMagicsAll();
            InitAll(boardState);
            ParsePosition("position startpos\n", boardState);
        }
        else if (!strncmp(line, "go", 2))
        {
            
            // printf("Seen Go..\n");
            ParseGo(line, boardState);
        }
    }
}
