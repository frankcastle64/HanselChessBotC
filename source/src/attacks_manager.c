
#include "position.h"
#include "types.h"
#include "attacks.h"
#include "pieces.h"
#include "bitboard.h"


U64 IsSquareAttacked(int square, Position *position)
{
    const int us = position->sideToMove;
    const int enemy = us ^ 1;
    const U64 occupied = ~position->piecesByType[EMPTY];
    U64 pieces = position->piecesByColor[enemy] & position->piecesByType[KNIGHT];

    if (pieces & KNIGHT_ATTACKS[square])
        return 1;

    pieces = position->piecesByColor[enemy] & position->piecesByType[KING];
    if (pieces & KING_ATTACKS[square])
        return 1;

    pieces = position->piecesByColor[enemy] & (position->piecesByType[BISHOP] | position->piecesByType[QUEEN]);
    if (pieces & GenerateBishopAttacks(square, occupied))
        return 1;

    pieces = position->piecesByColor[enemy] & (position->piecesByType[ROOK] | position->piecesByType[QUEEN]);
    if (pieces & GenerateRookAttacks(square, occupied))
        return 1;

    pieces = (((position->piecesByColor[enemy] & position->piecesByType[PAWN] & BB_NOT_FILE_A) << 7) >> (16 * enemy)) | (((position->piecesByColor[enemy] & position->piecesByType[PAWN] & BB_NOT_FILE_H) << 9) >> (16 * enemy));

    return pieces & (1ULL << square);
}

U64 IsCheck(Position *position)
{
    int kingSquare = __builtin_ctzll(position->piecesByType[KING] & position->piecesByColor[position->sideToMove]);
    U64 isAttacked = IsSquareAttacked(kingSquare, position);
    return isAttacked;
}