#ifndef MAGIC_BITBOARDS_H_INCLUDED
#define MAGIC_BITBOARDS_H_INCLUDED

#include "consts.h"
#include "types.h"

typedef struct Magic
{
    U64 mask;
    U64 magic;
    U64 *attacks;
    U32 shift;

} Magic;

extern Magic RookMagics[Number_Of_Squares];
extern Magic BishopMagics[Number_Of_Squares];

extern U64 RookTable[0x19000];  // To store rook attacks
extern U64 BishopTable[0x1480]; // To store bishop attacks

void InitMagicsAll();

#endif