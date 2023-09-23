#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "position.h"

void PrintBoard(BoardState *boardState);
void PrintPosition(Position *position);
void PrintMoveList(MoveList *moveList);
void PrintMove(Move *move);
void PrintMoveWithNodes(Move *move, int nodes);
#endif