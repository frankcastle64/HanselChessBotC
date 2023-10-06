#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "position.h"

void GenerateMoves(Position *position, MoveList *moveList);
void GenerateQSearchMoves(Position *position, MoveList *moveList);

void GenerateCaptureMoves(U64 emptySquares, U64 pawnsOfColor, U64 knightsOfColor, U64 bishopsOfColor, U64 rooksOfColor, U64 queensOfColor, U64 kingOfColor, U64 enemyPieces, U64 enpassantSquare, int sideToMove, int forwardDirection, MoveList *moveList);
void GenerateNonCaptureMoves(U64 emptySquares, U64 pawnsOfColor, U64 knightsOfColor, U64 bishopsOfColor, U64 rooksOfColor, U64 queensOfColor, U64 kingOfColor, int sideToMove, int forwardDirection, MoveList *moveList);
void GeneratePawnNonCapturesMoves(U64 pawnsOfColor, U64 emptySquares, int sideToMove, int forwardDirection, MoveList *moveList);



void GenerateKnightMoves(U64 knightsOfColor, U64 targetSquares, int sideToMove, MoveList *moveList);
void GenerateKingMoves(U64 kingOfColor, U64 targetSquares, int sideToMove, MoveList *moveList);
void GenerateBishopMoves(U64 bishopsOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList);
void GenerateRookMoves(U64 rooksOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList);
void GenerateQueenMoves(U64 queensOfColor, U64 targetSquares, U64 occupied, int sideToMove, MoveList *moveList);

//void GeneratePawnCapturesMoves(U64 pawnsOfColor, U64 enemyPieces, int sideToMove, int forwardDirection, MoveList *moveList);
 void GeneratePawnCapturesMoves(U64 pawnsOfColor, U64 enemyPieces, U64 enpassantSquare, int sideToMove, int forwardDirection, MoveList *moveList);

 void GenerateCastlingMoves(int sideToMove, U64 castlePermissions, U64 occupied, Position *position, MoveList*moveList);
#endif