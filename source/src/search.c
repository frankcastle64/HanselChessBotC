#include "search.h"
#include "position.h"
#include "types.h"
#include "move_manager.h"
#include "attacks_manager.h"
#include "display.h"
#include "move_gen.h"
#include "stdio.h"
#include "eval.h"
#include "pieces.h"
#include "consts.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
U64 nodes = 0ULL;

long long getCurrentTimeInMilliseconds()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000LL + currentTime.tv_usec / 1000LL;
}

void Think(Position *position, int depth)
{
    nodes = 0ULL;
    Move bestMove;
    long long t0 = getCurrentTimeInMilliseconds();
    int score = Search(position, depth, 0, -10000, 10000, &bestMove);
    long long t1 = getCurrentTimeInMilliseconds();

    PrintMove(&bestMove);
    printf("depth : %d\tscore : %d\tnodes :%lld\t%lld ms\t%f KNPS \n", depth, score, nodes, (t1 - t0), (1.0 * nodes / (t1 - t0)));
}

int QSearch(Position *position, int alpha, int beta)
{
    int stand_pat = Eval(position);
    if (stand_pat >= beta)
    {
        return beta;
    }
    if (alpha < stand_pat)
    {
        alpha = stand_pat;
    }

    MoveList moveList[1];
    GenerateQSearchMoves(position, moveList);
    // PrintMoveList(moveList);
    MVVLVASort(position, moveList);
    // printf("------------------------------------\n\n");
    // PrintMoveList(moveList);
    for (int i = 0; i < moveList->count; i++)
    {
        copy_board();
        MakeMove(position, &moveList->moves[i]);

        if (!IsCheck(position))
        {

            position->sideToMove ^= 1;
            int score = -QSearch(position, -beta, -alpha);
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
            }
        }
        take_back();
    }

    return alpha;
}

int Search(Position *position, int depth, int ply, int alpha, int beta, Move *bestMove)
{
    nodes++;
    if (depth == 0)
    {
        return QSearch(position, alpha, beta);
        // return Eval(position);
    }
    int legalMovePlayed = 0;
    MoveList moveList[1];
    GenerateMoves(position, moveList);
    // PrintMoveList(moveList);
    MVVLVASort(position, moveList);
    // printf("--------------------------------------------------\n\n");
    // PrintMoveList(moveList);
    for (int i = 0; i < moveList->count; i++)
    {
        copy_board();
        MakeMove(position, &moveList->moves[i]);

        if (!IsCheck(position))
        {
            legalMovePlayed = 1;
            position->sideToMove ^= 1;
            int score = -Search(position, depth - 1, ply + 1, -beta, -alpha, bestMove);
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
                if (!ply)
                {
                    *bestMove = moveList->moves[i];
                }
            }
        }
        take_back();
    }

    if (!legalMovePlayed)
    {
        U64 isCheck = IsCheck(position);
        if (isCheck)
        {
            return -9999 + ply;
        }
        else
        {
            return 0;
        }
    }
    return alpha;
}

// #define GetPiece(bb0, bb1, bb2, bb3, sq) ((bb0 >> sq) & 1) + 2 * ((bb1 >> sq) & 1) + 4 * ((bb2 >> sq) & 1) + 8 * ((bb3 >> sq) & 1)

// int CompareByMvvLvaScores(const void *a, const void *b)
// {
//     const struct Move *moveOne = (const struct Move *)a;
//     const struct Move *moveTwo = (const struct Move *)b;

//     return moveTwo->mvvLvaScore - moveOne->mvvLvaScore;
// }

void MVVLVASort(Position *position, MoveList *moveList)
{
    U64 bb0 = position->piecesByColor[COLOR_BLACK];
    U64 bb1 = position->piecesByType[PAWN] | position->piecesByType[BISHOP] | position->piecesByType[QUEEN];
    U64 bb2 = position->piecesByType[KNIGHT] | position->piecesByType[BISHOP] | position->piecesByType[KING];
    U64 bb3 = position->piecesByType[ROOK] | position->piecesByType[QUEEN] | position->piecesByType[KING];

    for (int index = 0; index < moveList->count; index++)
    {
        int attacker = moveList->moves[index].typeOfMove;
        int sq = moveList->moves[index].toSq;
        int capturedPiece = (((bb0 >> sq) & 1) + 2 * ((bb1 >> sq) & 1) + 4 * ((bb2 >> sq) & 1) + 8 * ((bb3 >> sq) & 1));
        int victim = ((capturedPiece >> 1));
        moveList->moves[index].mvvLvaScore = MVVLVAScores[attacker][victim];
    }

    int n = moveList->count;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (moveList->moves[j + 1].mvvLvaScore > moveList->moves[j].mvvLvaScore)
            {
                // Swap arr[j] and arr[j + 1]
                Move temp = moveList->moves[j + 1];
                moveList->moves[j + 1] = moveList->moves[j];
                moveList->moves[j] = temp;
            }
        }
    }

    // qsort(moveList->moves, moveList->count, sizeof(struct Move), CompareByMvvLvaScores);
}
