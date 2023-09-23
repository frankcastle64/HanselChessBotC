#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "position.h"
#include "types.h"

void Think(Position *position, int depth);
int Search(Position *position, int depth, int ply, int alpha, int beta, Move* bestMove);
int GetPiece(U64 bb0, U64 bb1, U64 bb2, U64 bb3, int sq);
void MVVLVASort(Position* position, MoveList *moveList);
#endif