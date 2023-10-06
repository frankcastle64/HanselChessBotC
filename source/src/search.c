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
Move PV[MAX_PLY][MAX_PLY];
int pv_length[MAX_PLY];
uint8_t followPV = 1;

long long getCurrentTimeInMilliseconds()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000LL + currentTime.tv_usec / 1000LL;
}

char *move_str(Move move)
{
    static char str[6];
    int sqFrom = move.fromSq;
    int sqTo = move.toSq;
    int sqFromFile = GetFileFromSquare(sqFrom);
    int sqFromRank = GetRankFromSquare(sqFrom) + 1;
    int sqToFile = GetFileFromSquare(sqTo);
    int sqToRank = GetRankFromSquare(sqTo) + 1;
    char promotedPiece = '\0';
    switch (move.typeOfMove)
    {
    case PROMOTION_KNIGHT:
        promotedPiece = 'n';
        break;
    case PROMOTION_BISHOP:
        promotedPiece = 'b';
        break;
    case PROMOTION_ROOK:
        promotedPiece = 'r';
        break;
    case PROMOTION_QUEEN:
        promotedPiece = 'q';
        break;
    default:
        promotedPiece = '\0';
        break;
    }
    sprintf(str, "%c%d%c%d%c\n", (char)(sqFromFile + 97), sqFromRank, (char)(sqToFile + 97), sqToRank, promotedPiece);

    return str;
}
void Think(Position *position, int depth)
{
    nodes = 0ULL;
    Move bestMove;
    int depth1 = 1;
    while (depth1 <= depth)
    {
        followPV = 1;
        memset(PV, 0, sizeof(PV));
        long long t0 = getCurrentTimeInMilliseconds();
        int score = Search(position, depth1, 0, -INF, INF, &bestMove);
        long long t1 = getCurrentTimeInMilliseconds();

        printf("info score cp %d depth %d nodes %lld time %lld ", score, depth1, nodes, (t1 - t0));
        printf("pv ");
        for (int j = 0; j < pv_length[0]; ++j)
            printf("%s ", move_str(PV[0][j]));
        printf("\n");
        depth1++;
    }

    printf("bestmove %s\n", move_str(PV[0][0]));
    // printf("\ndepth : %d\tscore : %d\tnodes :%lld\t%lld ms\t%f KNPS \n", depth, score, nodes, (t1 - t0), (1.0 * nodes / (t1 - t0)));
}

int QSearch(Position *position, int alpha, int beta, int ply)
{
    pv_length[ply] = ply;
    if (ply >= MAX_PLY - 1)
        return Eval(position);
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
    if (followPV) /* are we following the PV? */
        PVSort(moveList, ply);
    MVVLVASort(position, moveList);

    for (int i = 0; i < moveList->count; i++)
    {
        copy_board();
        MakeMove(position, &moveList->moves[i]);

        if (!IsCheck(position))
        {

            position->sideToMove ^= 1;
            int score = -QSearch(position, -beta, -alpha, ply + 1);
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
                /* update the PV */
                PV[ply][ply] = moveList->moves[i];
                for (int j = ply + 1; j < pv_length[ply + 1]; ++j)
                    PV[ply][j] = PV[ply + 1][j];
                pv_length[ply] = pv_length[ply + 1];
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
        return QSearch(position, alpha, beta, ply);
        // return Eval(position);
    }
    pv_length[ply] = ply;
    if (ply >= MAX_PLY - 1)
        return Eval(position);
    int legalMovePlayed = 0;
    MoveList moveList[1];
    GenerateMoves(position, moveList);
    if (followPV) /* are we following the PV? */
        PVSort(moveList, ply);
    MVVLVASort(position, moveList);
    U64 isCheck = IsCheck(position);
    if (isCheck)
        depth++;
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
                /* update the PV */
                PV[ply][ply] = moveList->moves[i];
                for (int j = ply + 1; j < pv_length[ply + 1]; ++j)
                    PV[ply][j] = PV[ply + 1][j];
                pv_length[ply] = pv_length[ply + 1];
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

void MVVLVASort(Position *position, MoveList *moveList)
{
    U64 bb0 = position->piecesByColor[COLOR_BLACK];
    U64 bb1 = position->piecesByType[PAWN] | position->piecesByType[BISHOP] | position->piecesByType[QUEEN];
    U64 bb2 = position->piecesByType[KNIGHT] | position->piecesByType[BISHOP] | position->piecesByType[KING];
    U64 bb3 = position->piecesByType[ROOK] | position->piecesByType[QUEEN] | position->piecesByType[KING];

    for (int index = 0; index < moveList->count; index++)
    {
        int sqFrom = moveList->moves[index].fromSq;
        int sqTo = moveList->moves[index].toSq;

        int fromPiece = (((bb0 >> sqFrom) & 1) + 2 * ((bb1 >> sqFrom) & 1) + 4 * ((bb2 >> sqFrom) & 1) + 8 * ((bb3 >> sqFrom) & 1));
        int toPiece = (((bb0 >> sqTo) & 1) + 2 * ((bb1 >> sqTo) & 1) + 4 * ((bb2 >> sqTo) & 1) + 8 * ((bb3 >> sqTo) & 1));

        int attacker = ((fromPiece >> 1));
        int victim = ((toPiece >> 1));

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

void PVSort(MoveList *moveList, int ply)
{
    int i;

    followPV = 0;
    for (i = 0; i < moveList->count; ++i)
        if (moveList->moves[i].fromSq == PV[0][ply].fromSq && moveList->moves[i].toSq == PV[0][ply].toSq && moveList->moves[i].fromPiece == PV[0][ply].fromPiece)
        {
            followPV = 1;
            moveList->moves[i].mvvLvaScore = 10000000;
            return;
        }
}
