
#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

#include "types.h"
#include "consts.h"

extern const U64 KNIGHT_ATTACKS[Number_Of_Squares];
extern const U64 KING_ATTACKS[Number_Of_Squares];

U64 GenerateBishopAttacks(int sqFrom, U64 occupied);
U64 GenerateRookAttacks(int sqFrom, U64 occupied);


#endif