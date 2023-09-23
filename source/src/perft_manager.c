#include "perft_manager.h"
#include "position.h"
#include "init.h"
#include "move_manager.h"
#include "move_gen.h"
#include "stdio.h"
#include "attacks_manager.h"
#include "display.h"

const int MAX_PLY = 10;

void PrintBB(Position *position)
{
    printf("Display bb :: \n");
    // Print bitboards
    for (int i = 0; i < 7; i++)
    {
        printf("0X%llX\n", position->piecesByType[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        printf("0x%llX\n", position->piecesByColor[i]);
    }
}

U64 Divide(Position *position, int depth, int ply)
{
    U64 nodes = 0;
    if (depth == 0)
    {
        return 1;
    }

    MoveList moveList[1];
    GenerateMoves(position, moveList);
    for (int i = 0; i < moveList->count; i++)
    {
        copy_board();
        MakeMove(position, &moveList->moves[i]);

        if (!IsCheck(position))
        {
            position->sideToMove ^= 1;
           // PrintMove(&moveList->moves[i]);
            nodes += Divide(position, depth - 1, ply + 1);
        }
        take_back();
    }

    return nodes;
}

U64 Perft(Position *position, int depth, int ply)
{
    U64 nodes = 0;
    if (depth == 0)
    {
        return 1;
    }

    MoveList moveList[1];
    GenerateMoves(position, moveList);
    for (int i = 0; i < moveList->count; i++)
    {
        U64 nodes_divide = 0;
        copy_board();
       //PrintPosition(position);
        MakeMove(position, &moveList->moves[i]);
       //PrintPosition(position);
        if (!IsCheck(position))
        {
            position->sideToMove ^= 1;
            nodes_divide = Divide(position, depth - 1, ply + 1);
            nodes += nodes_divide;
            //PrintMoveWithNodes(&moveList->moves[i], nodes_divide);
        }
        take_back();
    }

    return nodes;
}