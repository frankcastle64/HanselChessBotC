#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define WHITE_KING_SIDE_CASTLE 7
#define WHITE_QUEEN_SIDE_CASTLE 8
#define BLACK_KING_SIDE_CASTLE 9
#define BLACK_QUEEN_SIDE_CASTLE 10

#define PROMOTION_KNIGHT 11
#define PROMOTION_BISHOP 12
#define PROMOTION_ROOK 13
#define PROMOTION_QUEEN 14

#define MOVE_ENPASSANT 15
#define MOVE_CASTLE_KSIDE 16
#define MOVE_CASTLE_QSIDE 17

#define EMPTY  0
#define WP  2
#define WN  4
#define WB  6
#define WR  8
#define WQ  10
#define WK  12
#define BP  3
#define BN  5
#define BB  7
#define BR  9
#define BQ  11
#define BK  13

#define GetColorOfPiece(piece) ((piece & 1))
#define GetTypeOfPiece(piece) ((piece >> 1))
#define GetPieceIndex(pieceType) (pieceType-1)
#define GetMaterialPieceIndex(pieceIndex,square) (64*pieceIndex + square)
#endif