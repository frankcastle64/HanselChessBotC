#include "fen_manager.h"
#include <string.h>
#include <ctype.h>
#include "consts.h"
#include "pieces.h"

int IndexOf(const char *parentStr, char token)
{
    char *foundIndex = strchr(parentStr, token);
    return (int)(foundIndex - parentStr);
}

void UpdateBitBoards(BoardState *boardState)
{
    for (int square = 0; square <= 63; square++)
    {
        int piece = boardState->board[square];
        int pieceType = GetTypeOfPiece(piece);
        boardState->position->piecesByType[pieceType] |= (1ULL << square);
        if (piece != EMPTY)
        {
            int pieceColor = GetColorOfPiece(piece);
            boardState->position->piecesByColor[pieceColor] |= (1ULL << square);
        }
    }
}
void SetBoard(char *fen, BoardState *boardState)
{
    int rank = RANKS - 1, file = FILE_A, piece = EMPTY;
    for (; rank >= RANK_1 && *fen; fen++)
    {
        char p = *fen;
        if (isalpha(p))
        {
            piece = IndexOf(PIECES_STR, p);
            int square = Make_Square(file, rank);
            boardState->board[square] = piece;
            file = file + 1;
        }
        else if (isdigit(p))
        {
            file += (p - '0');
        }
        else
        {
            rank = rank - 1;
            file = FILE_A;
        }
    }

    // Set turn
    if (*fen == 'w')
    {
        boardState->position->sideToMove = 0;
    }
    else
    {
        boardState->position->sideToMove = 1;
    }

    fen = fen + 2;

    if (*fen == 'K')
    {
        boardState->position->castleRights[0] |= WhiteKingsideRookBit;
        fen++;
    }
    if (*fen == 'Q')
    {
        boardState->position->castleRights[0] |= WhiteQueensideRookBit;
        fen++;
    }
    if (*fen == 'k')
    {
        boardState->position->castleRights[1] |= BlackKingsideRookBit;
        fen++;
    }
    if (*fen == 'q')
    {
        boardState->position->castleRights[1] |= BlackQueensideRookBit;
        fen++;
    }
    if (*fen == '-')
    {
        boardState->position->castleRights[0] = 0;
        boardState->position->castleRights[1] = 0;
        fen++;
    }

    fen++;
    if (*fen != '-')
    {
        char f = *fen;
        fen++;
        char r = *fen;
        int sq = Make_Square((f - 'a'), (r - '1'));
        boardState->position->enpassantSq = (1ULL<<sq);
    }

    fen++;

    UpdateBitBoards(boardState);
}