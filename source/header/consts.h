#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#include "types.h"

extern const char *PIECES_STR;

#define RANKS 8
#define FILES 8
#define Number_Of_Squares RANKS *FILES

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

#define COLOR_WHITE 0
#define COLOR_BLACK 1

#define Make_Square(file, rank) (((rank) << 3) + file)
#define GetRankFromSquare(square) (square >> 3)
#define GetFileFromSquare(square) (square & 7)

#define BlackQueensideRookBit 0x0100000000000000ULL
#define BlackKingsideRookBit 0x8000000000000000ULL
#define WhiteQueensideRookBit 0x0000000000000001ULL
#define WhiteKingsideRookBit 0x0000000000000080ULL

extern const U64 CastleRightsConsts[64];
extern const int MVVLVAScores[7][7];

extern const U64 CastleBits[2][2];
extern const U64 CastleJump[2][2];
extern const int CastleJumpSquares[2][4];
extern const int CastleSquares[2][4];

#endif