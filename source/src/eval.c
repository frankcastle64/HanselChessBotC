

#include "eval.h"
#include "consts.h"
#include "pieces.h"
#include "types.h"
#include "position.h"

const int MaterialValues[7] = {0, 100, 300, 300, 500, 900, 1000};
int EvalByColor[2];
void EvalMaterial(Position *position)
{
    for (int color = COLOR_WHITE; color <= COLOR_BLACK; color++)
    {
        for (int pieceType = PAWN; pieceType <= KING; pieceType++)
        {
            U64 pieces = position->piecesByColor[color] & position->piecesByType[pieceType];
            int numberOfPieces = __builtin_popcountll(pieces);
            EvalByColor[color] += (numberOfPieces * MaterialValues[pieceType]);
        }
    }
}

int Eval(Position *position)
{
    EvalByColor[COLOR_WHITE] = 0;
    EvalByColor[COLOR_BLACK] = 0;
    EvalMaterial(position);
    int eval = EvalByColor[COLOR_WHITE] - EvalByColor[COLOR_BLACK];
    return eval * (1 - (position->sideToMove << 1));
}