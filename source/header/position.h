#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "types.h"

typedef struct Position
{
    U64 piecesByColor[2];
    U64 piecesByType[8];
    U64 castleRights[2];
    int sideToMove;
    U64 enpassantSq;

} Position;

typedef struct BoardState
{
    int board[64];
    Position position[1];
} BoardState;

typedef struct Move
{
    int fromSq;
    int toSq;
    int capturedPiece;
    int typeOfMove;
    int mvvLvaScore;
} Move;
typedef struct MoveList
{
    Move moves[256];
    int count;
} MoveList;

#endif