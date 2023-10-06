#include "move_gen.h"
#include "position.h"
#include "types.h"
#include "pieces.h"
#include "bitboard.h"
#include "attacks.h"
#include "consts.h"
#include "squares.h"
#include "attacks_manager.h"

void AddMove(MoveList *moveList, int sqFrom, int sqTo, int typeOfMove, int capturedPiece)
{
    moveList->moves[moveList->count].fromSq = sqFrom;
    moveList->moves[moveList->count].toSq = sqTo;
    moveList->moves[moveList->count].typeOfMove = typeOfMove;
    moveList->moves[moveList->count].capturedPiece = capturedPiece;
    moveList->count++;
}

void LoopPawnNonPromoMoves(U64 movesBitBoard, int offset, int typeOfMove, MoveList *moveList)
{
    for (; movesBitBoard > 0; movesBitBoard &= movesBitBoard - 1)
    {
        int sqTo = __builtin_ctzll(movesBitBoard);
        int sqFrom = sqTo + offset;
        AddMove(moveList, sqFrom, sqTo, typeOfMove, EMPTY);
    }
}

void LoopPromoMoves(U64 movesBitBoard, int offset, MoveList *moveList)
{
    for (; movesBitBoard > 0; movesBitBoard &= movesBitBoard - 1)
    {
        int sqTo = __builtin_ctzll(movesBitBoard);
        int sqFrom = sqTo + offset;
        AddMove(moveList, sqFrom, sqTo, PROMOTION_KNIGHT, EMPTY);
        AddMove(moveList, sqFrom, sqTo, PROMOTION_BISHOP, EMPTY);
        AddMove(moveList, sqFrom, sqTo, PROMOTION_ROOK, EMPTY);
        AddMove(moveList, sqFrom, sqTo, PROMOTION_QUEEN, EMPTY);
    }
}

void GenerateQSearchMoves(Position *position, MoveList *moveList)
{
    int sideToMove = position->sideToMove;
    moveList->count = 0;
    const U64 enpassantSquare = position->enpassantSq;

    const U64 piecesOfColor = position->piecesByColor[sideToMove];
    const U64 enemyPieces = position->piecesByColor[(sideToMove ^ 1)];

    const U64 emptySquares = position->piecesByType[EMPTY];
    const U64 pawnsOfColor = piecesOfColor & position->piecesByType[PAWN];
    const U64 knightsOfColor = piecesOfColor & position->piecesByType[KNIGHT];
    const U64 bishopsOfColor = piecesOfColor & position->piecesByType[BISHOP];
    const U64 rooksOfColor = piecesOfColor & position->piecesByType[ROOK];
    const U64 queensOfColor = piecesOfColor & position->piecesByType[QUEEN];
    const U64 kingOfColor = piecesOfColor & position->piecesByType[KING];
    int forwardDirection = 8 - (16 * sideToMove);
    GenerateCaptureMoves(emptySquares, pawnsOfColor, knightsOfColor, bishopsOfColor, rooksOfColor, queensOfColor, kingOfColor, enemyPieces, enpassantSquare, sideToMove, forwardDirection, moveList);
}
void GenerateMoves(Position *position, MoveList *moveList)
{
    int sideToMove = position->sideToMove;
    moveList->count = 0;
    const U64 enpassantSquare = position->enpassantSq;
    const U64 castlePermissions = position->castleRights[sideToMove];
    const U64 piecesOfColor = position->piecesByColor[sideToMove];
    const U64 enemyPieces = position->piecesByColor[(sideToMove ^ 1)];

    const U64 emptySquares = position->piecesByType[EMPTY];
    const U64 occupied = ~emptySquares;
    const U64 pawnsOfColor = piecesOfColor & position->piecesByType[PAWN];
    const U64 knightsOfColor = piecesOfColor & position->piecesByType[KNIGHT];
    const U64 bishopsOfColor = piecesOfColor & position->piecesByType[BISHOP];
    const U64 rooksOfColor = piecesOfColor & position->piecesByType[ROOK];
    const U64 queensOfColor = piecesOfColor & position->piecesByType[QUEEN];
    const U64 kingOfColor = piecesOfColor & position->piecesByType[KING];

    int forwardDirection = 8 - (16 * sideToMove);

    GenerateCaptureMoves(emptySquares, pawnsOfColor, knightsOfColor, bishopsOfColor, rooksOfColor, queensOfColor, kingOfColor, enemyPieces, enpassantSquare, sideToMove, forwardDirection, moveList);
    GenerateNonCaptureMoves(emptySquares, pawnsOfColor, knightsOfColor, bishopsOfColor, rooksOfColor, queensOfColor, kingOfColor, sideToMove, forwardDirection, moveList);
    GenerateCastlingMoves(sideToMove, castlePermissions, occupied, position, moveList);
}

void GenerateCaptureMoves(U64 emptySquares, U64 pawnsOfColor, U64 knightsOfColor, U64 bishopsOfColor, U64 rooksOfColor, U64 queensOfColor, U64 kingOfColor, U64 enemyPieces, U64 enpassantSquare, int sideToMove, int forwardDirection, MoveList *moveList)
{
    GeneratePawnCapturesMoves(pawnsOfColor, enemyPieces, enpassantSquare, sideToMove, forwardDirection, moveList);
    GenerateKnightMoves(knightsOfColor, enemyPieces, sideToMove, moveList);
    GenerateKingMoves(kingOfColor, enemyPieces, sideToMove, moveList);
    GenerateBishopMoves(bishopsOfColor, enemyPieces, ~emptySquares, sideToMove, moveList);
    GenerateRookMoves(rooksOfColor, enemyPieces, ~emptySquares, sideToMove, moveList);
    GenerateQueenMoves(queensOfColor, enemyPieces, ~emptySquares, sideToMove, moveList);
}

void GenerateNonCaptureMoves(U64 emptySquares, U64 pawnsOfColor, U64 knightsOfColor, U64 bishopsOfColor, U64 rooksOfColor, U64 queensOfColor, U64 kingOfColor, int sideToMove, int forwardDirection, MoveList *moveList)
{
    GeneratePawnNonCapturesMoves(pawnsOfColor, emptySquares, sideToMove, forwardDirection, moveList);
    GenerateKnightMoves(knightsOfColor, emptySquares, sideToMove, moveList);
    GenerateKingMoves(kingOfColor, emptySquares, sideToMove, moveList);
    GenerateBishopMoves(bishopsOfColor, emptySquares, ~emptySquares, sideToMove, moveList);
    GenerateRookMoves(rooksOfColor, emptySquares, ~emptySquares, sideToMove, moveList);
    GenerateQueenMoves(queensOfColor, emptySquares, ~emptySquares, sideToMove, moveList);
}

void GeneratePawnNonCapturesMoves(U64 pawnsOfColor, U64 emptySquares, int sideToMove, int forwardDirection, MoveList *moveList)
{
    U64 pawnMoves = ((pawnsOfColor << 8) >> (16 * sideToMove)) & emptySquares;
    // Pawn promotions.
    U64 promotingPawns = pawnMoves & SIDE_RANKS[sideToMove][RANK_8];
    // Single push.
    U64 singlePawnPush = pawnMoves ^ promotingPawns;
    // Double push candidates.
    U64 doublePawnPush = (((singlePawnPush & SIDE_RANKS[sideToMove][RANK_3]) << 8) >> (16 * sideToMove)) & emptySquares;
    // Add them in moveList.
    LoopPromoMoves(promotingPawns, -forwardDirection, moveList);
    LoopPawnNonPromoMoves(singlePawnPush, -forwardDirection, PAWN, moveList);
    LoopPawnNonPromoMoves(doublePawnPush, -(forwardDirection << 1), PAWN, moveList);
}

void GenerateKnightMoves(U64 knightsOfColor, U64 targetSquares, int sideToMove, MoveList *moveList)
{
    for (; knightsOfColor > 0; knightsOfColor &= knightsOfColor - 1)
    {
        int sqFrom = __builtin_ctzll(knightsOfColor);
        int capturedPiece = EMPTY;

        U64 knightAttacksFromSquare = KNIGHT_ATTACKS[sqFrom] & targetSquares;

        for (; knightAttacksFromSquare > 0; knightAttacksFromSquare &= knightAttacksFromSquare - 1)
        {
            int sqTo = __builtin_ctzll(knightAttacksFromSquare);
            AddMove(moveList, sqFrom, sqTo, KNIGHT, capturedPiece);
        }
    }
}

void GenerateKingMoves(U64 kingOfColor, U64 targetSquares, int sideToMove, MoveList *moveList)
{
    for (; kingOfColor > 0; kingOfColor &= kingOfColor - 1)
    {
        int sqFrom = __builtin_ctzll(kingOfColor);
        U64 kingAttacksFromSquare = KING_ATTACKS[sqFrom] & targetSquares;

        for (; kingAttacksFromSquare > 0; kingAttacksFromSquare &= kingAttacksFromSquare - 1)
        {
            int sqTo = __builtin_ctzll(kingAttacksFromSquare);
            AddMove(moveList, sqFrom, sqTo, KING, EMPTY);
        }
    }
}

void GenerateBishopMoves(U64 bishopsOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList)
{
    for (; bishopsOfColor > 0; bishopsOfColor &= bishopsOfColor - 1)
    {
        int sqFrom = __builtin_ctzll(bishopsOfColor);
        U64 bishopAttacksFromSquare = GenerateBishopAttacks(sqFrom, occupied) & targetSquares;
        for (; bishopAttacksFromSquare > 0; bishopAttacksFromSquare &= bishopAttacksFromSquare - 1)
        {
            int sqTo = __builtin_ctzll(bishopAttacksFromSquare);
            AddMove(moveList, sqFrom, sqTo, BISHOP, EMPTY);
        }
    }
}

void GenerateRookMoves(U64 rooksOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList)
{
    for (; rooksOfColor > 0; rooksOfColor &= rooksOfColor - 1)
    {
        int sqFrom = __builtin_ctzll(rooksOfColor);
        U64 rookAttacksFromSquare = GenerateRookAttacks(sqFrom, occupied) & targetSquares;
        for (; rookAttacksFromSquare > 0; rookAttacksFromSquare &= rookAttacksFromSquare - 1)
        {
            int sqTo = __builtin_ctzll(rookAttacksFromSquare);
            AddMove(moveList, sqFrom, sqTo, ROOK, EMPTY);
        }
    }
}
void GenerateQueenMoves(U64 queensOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList)
{

    for (; queensOfColor > 0; queensOfColor &= queensOfColor - 1)
    {
        int sqFrom = __builtin_ctzll(queensOfColor);

        U64 queenAttacksFromSquare = (GenerateRookAttacks(sqFrom, occupied) | GenerateBishopAttacks(sqFrom, occupied)) & targetSquares;

        for (; queenAttacksFromSquare > 0; queenAttacksFromSquare &= queenAttacksFromSquare - 1)
        {
            int sqTo = __builtin_ctzll(queenAttacksFromSquare);
            AddMove(moveList, sqFrom, sqTo, QUEEN, EMPTY);
        }
    }
}

// Generate capture moves.
void GeneratePawnCapturesMoves(U64 pawnsOfColor, U64 enemyPieces, U64 enpassantSquare, int sideToMove, int forwardDirection, MoveList *moveList)
{
    // Consider normal promotions here as well as promotions after captures since they are not quiet moves.

    // Capture left from white's perspective.
    U64 pawnCaptures = (((pawnsOfColor & BB_NOT_FILE_A) << 7) >> (16 * sideToMove)) & (enemyPieces);
    U64 pawnCapturesPromoting = pawnCaptures & SIDE_RANKS[sideToMove][RANK_8];
    U64 pawnCapturesNonPromoting = pawnCaptures ^ pawnCapturesPromoting;
    U64 epCapture = (((pawnsOfColor & ENPASSANT_RANKS_BB[sideToMove][0]) << 7) >> (16 * sideToMove)) & enpassantSquare;

    LoopPawnNonPromoMoves(pawnCapturesNonPromoting, (-forwardDirection + 1), PAWN, moveList);
    LoopPawnNonPromoMoves(epCapture, (-forwardDirection + 1), MOVE_ENPASSANT, moveList);
    LoopPromoMoves(pawnCapturesPromoting, (-forwardDirection + 1), moveList);

    // Capture right from whites perspective.
    pawnCaptures = (((pawnsOfColor & BB_NOT_FILE_H) << 9) >> (16 * sideToMove)) & (enemyPieces);
    pawnCapturesPromoting = pawnCaptures & SIDE_RANKS[sideToMove][RANK_8];
    pawnCapturesNonPromoting = pawnCaptures ^ pawnCapturesPromoting;
    epCapture = (((pawnsOfColor & ENPASSANT_RANKS_BB[sideToMove][1]) << 9) >> (16 * sideToMove)) & enpassantSquare;

    LoopPawnNonPromoMoves(pawnCapturesNonPromoting, (-forwardDirection - 1), PAWN, moveList);
    LoopPawnNonPromoMoves(epCapture, (-forwardDirection - 1), MOVE_ENPASSANT, moveList);
    LoopPromoMoves(pawnCapturesPromoting, (-forwardDirection - 1), moveList);
}

// Generate castling moves.
void GenerateCastlingMoves(int sideToMove, U64 castlePermissions, U64 occupied, Position *position, MoveList *moveList)
{

    if ((castlePermissions & CastleBits[sideToMove][0]) 
        && (!(occupied & CastleJump[sideToMove][0])) 
        && (!IsSquareAttacked(CastleJumpSquares[sideToMove][0], position)) 
        && (!IsSquareAttacked(CastleJumpSquares[sideToMove][1], position)))
    {
        moveList->moves[moveList->count].typeOfMove = MOVE_CASTLE_QSIDE;
        moveList->moves[moveList->count].fromSq = CastleSquares[sideToMove][0];
        moveList->moves[moveList->count].toSq = CastleSquares[sideToMove][1];
        moveList->count++;
    }

    if ((castlePermissions & CastleBits[sideToMove][1]) && (!(occupied & CastleJump[sideToMove][1])) && (!IsSquareAttacked(CastleJumpSquares[sideToMove][2], position)) && (!IsSquareAttacked(CastleJumpSquares[sideToMove][3], position)))
    {
        moveList->moves[moveList->count].typeOfMove = MOVE_CASTLE_KSIDE;
        moveList->moves[moveList->count].fromSq = CastleSquares[sideToMove][2];
        moveList->moves[moveList->count].toSq = CastleSquares[sideToMove][3];
        moveList->count++;
    }
}