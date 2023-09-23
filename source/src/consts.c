#include "consts.h"
#include "types.h"
#include "squares.h"

const char *PIECES_STR = "..PpNnBbRrQqKk";

const int MVVLVAScores[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 105, 205, 305, 405, 505, 605},
    {0, 104, 204, 304, 404, 504, 604},
    {0, 103, 203, 303, 403, 503, 603},
    {0, 102, 202, 302, 402, 502, 602},
    {0, 101, 201, 301, 401, 501, 601},
    {0, 100, 200, 300, 400, 500, 600}};

const U64 CastleRightsConsts[64] = {
    WhiteQueensideRookBit, 0, 0, 0, 0x81ULL, 0, 0, WhiteKingsideRookBit,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    BlackQueensideRookBit, 0, 0, 0, 0x8100000000000000ULL, 0, 0, BlackKingsideRookBit};

const U64 CastleBits[2][2] = {{WhiteQueensideRookBit, WhiteKingsideRookBit}, {BlackQueensideRookBit, BlackKingsideRookBit}};
const U64 CastleJump[2][2] = {{0xeULL, 0x60ULL}, {0xe00000000000000ULL, 0x6000000000000000ULL}};
const int CastleJumpSquares[2][4] = {{SQ_E1, SQ_D1, SQ_E1, SQ_F1}, {SQ_E8, SQ_D8, SQ_E8, SQ_F8}};
const int CastleSquares[2][4] = {{SQ_E1, SQ_C1, SQ_E1, SQ_G1}, {SQ_E8, SQ_C8, SQ_E8, SQ_G8}};