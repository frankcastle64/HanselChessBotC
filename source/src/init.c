#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "init.h"
#include "consts.h"
#include "position.h"
#include "types.h"
#include "bitboard.h"
#include "magic_bitboards.h"
#include "pieces.h"

void InitAll(BoardState *boardState)
{
   memset(boardState->board, 0, Number_Of_Squares * sizeof(int));
   memset(boardState->position->piecesByType, 0, 8 * sizeof(U64));
   memset(boardState->position->piecesByColor, 0, 2 * sizeof(U64));
   memset(boardState->position->castleRights, 0, 2 * sizeof(U64));
   boardState->position->enpassantSq = 0;

   
}



#endif