#ifndef MOVE_MANAGER_H_INCLUDED
#define MOVE_MANAGER_H_INCLUDED

#include "position.h"
#include "types.h"
#include <string.h>

void MakeMove(Position *position, Move *move);
void MakeStrMove(char *move, Position *position);

// preserve board state
#define copy_board()           \
    Position position_copy[1]; \
    memcpy(position_copy, position, sizeof(Position));

// restore board state
#define take_back() \
    memcpy(position, position_copy, sizeof(Position));

#endif