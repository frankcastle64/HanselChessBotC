#include <stdio.h>
#include "display.h"
#include "consts.h"
#include "position.h"
#include "string.h"
#include "pieces.h"

void PrintBoard(BoardState *boardState)
{

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {

        for (int file = FILE_A; file <= FILE_H; file++)
        {

            int square = Make_Square(file, rank);
            printf(" %c ", PIECES_STR[boardState->board[square]]);
        }
        printf("\n");
    }

    printf("EP: %lld\n", boardState->position->enpassantSq);

    // printf("Display bb :: \n");
    // // Print bitboards
    // for (int i = 0; i < 7; i++)
    // {
    //     printf("0X%llX\n", boardState->position->piecesByType[i]);
    // }

    // for (int i = 0; i < 2; i++)
    // {
    //     printf("0x%llX\n", boardState->position->piecesByColor[i]);
    // }

    // for (int i = 0; i < 2; i++)
    // {
    //     printf("0x%llX\n", boardState->position->castleRights[i]);
    // }
}

void PrintPosition(Position *position)
{
    BoardState boardState[1];
    memcpy(boardState->position, position, sizeof(Position));

    // U64 whitePawns = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[PAWN];
    // U64 blackPawns = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[PAWN];
    // U64 whiteKnights = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[KNIGHT];
    // U64 blackKnights = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[KNIGHT];
    // U64 whiteBishops = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[BISHOP];
    // U64 blackBishops = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[BISHOP];
    // U64 whiteRooks = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[ROOK];
    // U64 blackRooks = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[ROOK];
    // U64 whiteQueens = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[QUEEN];
    // U64 blackQueens = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[QUEEN];
    // U64 whiteKing = boardState->position->piecesByColor[COLOR_WHITE] & boardState->position->piecesByType[KING];
    // U64 blackKing = boardState->position->piecesByColor[COLOR_BLACK] & boardState->position->piecesByType[KING];

    for (int i = 0; i < 64; i++)
    {
        boardState->board[i] = EMPTY;
    }

    for (int c = 0; c < 2; c++)
    {
        for (int p = PAWN; p <= KING; p++)
        {
            U64 pBB = position->piecesByColor[c] & position->piecesByType[p];
            while (pBB > 0)
            {
                int index = __builtin_ctzll(pBB);
                boardState->board[index] = ((2 * p) + c);
                pBB &= pBB - 1;
            }
        }
    }

    PrintBoard(boardState);
}

void PrintMoveList(MoveList *moveList)
{
    int count = moveList->count;
    for (int i = 0; i < count; i++)
    {
        PrintMove(&moveList->moves[i]);
    }
}

void PrintMove(Move *move)
{
    int sqFrom = move->fromSq;
    int sqTo = move->toSq;
    int sqFromFile = GetFileFromSquare(sqFrom);
    int sqFromRank = GetRankFromSquare(sqFrom) + 1;
    int sqToFile = GetFileFromSquare(sqTo);
    int sqToRank = GetRankFromSquare(sqTo) + 1;
    char promotedPiece = '\0';
    switch (move->typeOfMove)
    {
    case PROMOTION_KNIGHT:
        promotedPiece = 'n';
        break;
    case PROMOTION_BISHOP:
        promotedPiece = 'b';
        break;
    case PROMOTION_ROOK:
        promotedPiece = 'r';
        break;
    case PROMOTION_QUEEN:
        promotedPiece = 'q';
        break;
    default:
        promotedPiece = '\0';
        break;
    }
    printf("%c%d%c%d%c\n", (char)(sqFromFile + 97), sqFromRank, (char)(sqToFile + 97), sqToRank, promotedPiece);
}

void PrintMoveWithNodes(Move *move, int nodes)
{
    int sqFrom = move->fromSq;
    int sqTo = move->toSq;
    int sqFromFile = GetFileFromSquare(sqFrom);
    int sqFromRank = GetRankFromSquare(sqFrom) + 1;
    int sqToFile = GetFileFromSquare(sqTo);
    int sqToRank = GetRankFromSquare(sqTo) + 1;
    printf("%c%d%c%d %d\n", (char)(sqFromFile + 97), sqFromRank, (char)(sqToFile + 97), sqToRank, nodes);
}