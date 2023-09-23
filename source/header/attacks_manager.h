
#ifndef ATTACKS_MANAGER_H_INCLUDED
#define ATTACKS_MANAGER_H_INCLUDED

#include "position.h"
#include "types.h"

U64 IsSquareAttacked(int square, Position *position);
U64 IsCheck(Position *position);


#endif