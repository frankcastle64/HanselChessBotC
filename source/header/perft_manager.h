#ifndef PERFT_MANAGER_H_INCLUDED
#define PERFT_MANAGER_H_INCLUDED

#include "position.h"
#include "types.h"

U64 Perft(Position *position, int depth, int ply);

#endif