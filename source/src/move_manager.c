#include "move_manager.h"
#include "position.h"
#include "types.h"
#include "consts.h"
#include "pieces.h"
#include "bitboard.h"
#include "display.h"
#include "squares.h"
#include "move_gen.h"

void MakeMove(Position *position, Move *move)
{
    const U64 from_sq_bb = (1ULL << move->fromSq);
    const U64 to_sq_bb = (1ULL << move->toSq);
    const int type_of_move = move->typeOfMove;
    const int us = position->sideToMove;
    const int enemy = us ^ 1;
    position->piecesByColor[enemy] &= ~to_sq_bb;
    position->piecesByColor[us] ^= (from_sq_bb | to_sq_bb);
    position->enpassantSq = 0ULL;
    for (int pieceType = PAWN; pieceType <= KING; pieceType++)
    {
        position->piecesByType[pieceType] &= ~to_sq_bb;
    }

    position->castleRights[enemy] &= ~CastleRightsConsts[move->toSq];
    // position->castleRights[us] &= ~CastleRightsConsts[move->fromSq];

    switch (type_of_move)
    {
    case PAWN:
        position->piecesByType[PAWN] ^= (from_sq_bb | to_sq_bb);
        position->enpassantSq = (((from_sq_bb << 8) >> (16 * us)) & ((to_sq_bb >> 8) << (16 * us)));
        break;

    case KNIGHT:
        position->piecesByType[KNIGHT] ^= (from_sq_bb | to_sq_bb);
        break;

    case BISHOP:
        position->piecesByType[BISHOP] ^= (from_sq_bb | to_sq_bb);
        break;

    case ROOK:
        position->piecesByType[ROOK] ^= (from_sq_bb | to_sq_bb);
        position->castleRights[us] &= ~CastleRightsConsts[move->fromSq];
        break;

    case QUEEN:
        position->piecesByType[QUEEN] ^= (from_sq_bb | to_sq_bb);
        break;

    case KING:
        position->piecesByType[KING] ^= (from_sq_bb | to_sq_bb);
        position->castleRights[us] &= ~CastleRightsConsts[move->fromSq];
        break;

    case PROMOTION_KNIGHT:
        position->piecesByType[PAWN] ^= (from_sq_bb);
        position->piecesByType[KNIGHT] ^= (to_sq_bb);
        break;

    case PROMOTION_BISHOP:
        position->piecesByType[PAWN] ^= (from_sq_bb);
        position->piecesByType[BISHOP] ^= (to_sq_bb);
        break;

    case PROMOTION_ROOK:
        position->piecesByType[PAWN] ^= (from_sq_bb);
        position->piecesByType[ROOK] ^= (to_sq_bb);
        break;

    case PROMOTION_QUEEN:
        position->piecesByType[PAWN] ^= (from_sq_bb);
        position->piecesByType[QUEEN] ^= (to_sq_bb);
        break;

    case MOVE_ENPASSANT:
        position->piecesByType[PAWN] ^= from_sq_bb | to_sq_bb | ((to_sq_bb >> 8) << (16 * us));
        position->piecesByColor[enemy] &= ~((to_sq_bb >> 8) << (16 * us));
        break;

    case MOVE_CASTLE_KSIDE:
        position->piecesByType[KING] ^= from_sq_bb | to_sq_bb;
        position->castleRights[us] &= ~CastleRightsConsts[move->fromSq];
        position->piecesByType[ROOK] ^= CASTLED_ROOK_FILES_BB[us][0];
        position->piecesByColor[us] ^= CASTLED_ROOK_FILES_BB[us][0];
        break;

    case MOVE_CASTLE_QSIDE:
        position->piecesByType[KING] ^= from_sq_bb | to_sq_bb;
        position->castleRights[us] &= ~CastleRightsConsts[move->fromSq];
        position->piecesByType[ROOK] ^= CASTLED_ROOK_FILES_BB[us][1];
        position->piecesByColor[us] ^= CASTLED_ROOK_FILES_BB[us][1];
        break;

    default:
        break;
    }

    position->piecesByType[EMPTY] = ~(position->piecesByColor[COLOR_WHITE] | position->piecesByColor[COLOR_BLACK]);
}

// code from vice.
void MakeStrMove(char *move, Position *position)
{
    int sqFrom = Make_Square(move[0] - 'a', move[1] - '1');
    int sqTo = Make_Square(move[2] - 'a', move[3] - '1');
    MoveList moveList[1];
    GenerateMoves(position, moveList);
    int move_made = 0;
    for (int i = 0; i < moveList->count && !move_made; i++)
    {
        Move gen_move = moveList->moves[i];

        {
            switch (move[4])
            {
            case 'n':
                if (gen_move.fromSq == sqFrom && gen_move.toSq == sqTo && gen_move.typeOfMove == PROMOTION_KNIGHT)
                {
                    MakeMove(position, &gen_move);
                    move_made = 1;
                    position->sideToMove ^= 1;
                }
                break;
            case 'b':
                if (gen_move.fromSq == sqFrom && gen_move.toSq == sqTo && gen_move.typeOfMove == PROMOTION_BISHOP)
                {
                    MakeMove(position, &gen_move);
                    move_made = 1;
                    position->sideToMove ^= 1;
                }
                break;
            case 'r':
                if (gen_move.fromSq == sqFrom && gen_move.toSq == sqTo && gen_move.typeOfMove == PROMOTION_ROOK)
                {
                    MakeMove(position, &gen_move);
                    move_made = 1;
                    position->sideToMove ^= 1;
                }
                break;
            case 'q':
                if (gen_move.fromSq == sqFrom && gen_move.toSq == sqTo && gen_move.typeOfMove == PROMOTION_QUEEN)
                {
                    MakeMove(position, &gen_move);
                    move_made = 1;
                    position->sideToMove ^= 1;
                }
                break;
            default:
                if (gen_move.fromSq == sqFrom && gen_move.toSq == sqTo)
                {
                    MakeMove(position, &gen_move);
                    move_made = 1;
                    position->sideToMove ^= 1;
                }
                break;
            }
        }
    }
}