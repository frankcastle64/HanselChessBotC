#include <stdio.h>
#include <stdlib.h>
#include "magic_bitboards.h"
#include "types.h"
#include "squares.h"
#include "bitboard.h"
#include "pieces.h"
// U32 index(U64 occupied)
// {
//     return U32(((occupied & mask) * magic) >> shift);
// }

// Code taken from chessprogramming wiki.

U64 RookTable[0x19000];  // To store rook attacks
U64 BishopTable[0x1480]; // To store bishop attacks
Magic RookMagics[Number_Of_Squares];
Magic BishopMagics[Number_Of_Squares];

U64 GetSlidingAttacksRook(int sq, U64 block)
{

    U64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1; r <= 7; r++)
    {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8)))
            break;
    }
    for (r = rk - 1; r >= 0; r--)
    {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8)))
            break;
    }
    for (f = fl + 1; f <= 7; f++)
    {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8)))
            break;
    }
    for (f = fl - 1; f >= 0; f--)
    {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8)))
            break;
    }
    return result;
}

// Code taken from chessprogramming wiki.
U64 GetSlidingAttacksBishop(int sq, U64 block)
{
    U64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++)
    {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8)))
            break;
    }
    for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--)
    {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8)))
            break;
    }
    for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++)
    {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8)))
            break;
    }
    for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--)
    {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8)))
            break;
    }
    return result;
}

U64 random_uint64()
{
    U64 u1, u2, u3, u4;
    u1 = (U64)(rand()) & 0xFFFF;
    u2 = (U64)(rand()) & 0xFFFF;
    u3 = (U64)(rand()) & 0xFFFF;
    u4 = (U64)(rand()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 random_uint64_fewbits()
{
    return random_uint64() & random_uint64() & random_uint64();
}

void InitMagics(int pieceType, U64 *table, Magic *magics)
{

    U64 occupancy[4096], reference[4096], edges, b;
    int epoch[4096] = {}, cnt = 0, size = 0;

    for (int s = SQ_A1; s <= SQ_H8; s++)
    {
        U64 rankOfSquare = GetRankFromSquare(s);
        U64 currentRankBB = BB_RANK_1 << (8 * rankOfSquare);

        U64 fileOfSquare = GetFileFromSquare(s);
        U64 currentFileBB = BB_FILE_A << (fileOfSquare);

        edges = ((BB_RANK_1 | BB_RANK_8) & ~currentRankBB) | ((BB_FILE_A | BB_FILE_H) & ~currentFileBB);

        Magic *m = &magics[s];
        m->mask = (pieceType == ROOK ? GetSlidingAttacksRook(s, 0) : GetSlidingAttacksBishop(s, 0)) & ~edges;
        m->shift = 64 - __builtin_popcountll(m->mask);
        m->attacks = s == SQ_A1 ? table : magics[s - 1].attacks + size;
        b = size = 0;
        do
        {
            occupancy[size] = b;
            reference[size] = pieceType == ROOK ? GetSlidingAttacksRook(s, b) : GetSlidingAttacksBishop(s, b);
            size++;
            b = (b - m->mask) & m->mask;
        } while (b > 0);

        for (int i = 0; i < size;)
        {
            for (m->magic = 0; __builtin_popcountll((m->magic * m->mask) >> 56) < 6;)
            {

                m->magic = random_uint64_fewbits();
            }

            for (++cnt, i = 0; i < size; ++i)
            {
                U32 idx = (((occupancy[i] & m->mask) * m->magic) >> m->shift);

                if (epoch[idx] < cnt)
                {
                    epoch[idx] = cnt;
                    m->attacks[idx] = reference[i];
                }
                else if (m->attacks[idx] != reference[i])
                    break;
            }
        }
    }
}

void InitMagicsAll()
{
    InitMagics(ROOK, RookTable, RookMagics);
    InitMagics(BISHOP, BishopTable, BishopMagics);
}