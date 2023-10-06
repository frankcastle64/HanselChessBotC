#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "types.h"
#include "stdint.h"

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
    uint8_t  fromSq;
    uint8_t  toSq;
    uint8_t  capturedPiece;
    uint8_t  typeOfMove;
    uint8_t fromPiece;
    uint8_t toPiece;
    int mvvLvaScore;
} Move;

typedef struct MoveList
{
    Move moves[256];
    int count;
} MoveList;


#endif