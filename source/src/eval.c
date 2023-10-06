

#include "eval.h"
#include "consts.h"
#include "pieces.h"
#include "types.h"
#include "position.h"

const int MiddleGameMaterialValues[6 * 64] = {
    // Pawn
    100, 100, 100, 100, 100, 100, 100, 100,
    150, 150, 150, 150, 150, 150, 150, 150,
    110, 110, 120, 130, 130, 120, 110, 110,
    105, 105, 110, 125, 125, 110, 105, 105,
    100, 100, 100, 120, 120, 100, 100, 100,
    105, 95, 90, 100, 100, 90, 95, 105,
    105, 110, 110, 80, 80, 110, 110, 105,
    100, 100, 100, 100, 100, 100, 100, 100,

    // Knights
    250, 260, 270, 270, 270, 270, 260, 250,
    260, 280, 300, 300, 300, 300, 280, 260,
    270, 300, 310, 315, 315, 310, 300, 270,
    270, 305, 315, 320, 320, 315, 305, 270,
    270, 300, 315, 320, 320, 315, 300, 270,
    270, 305, 310, 315, 315, 310, 305, 270,
    260, 280, 300, 305, 305, 300, 280, 260,
    250, 260, 270, 270, 270, 270, 260, 250,

    // Bishop
    300, 310, 310, 310, 310, 310, 310, 300,
    310, 320, 320, 320, 320, 320, 320, 310,
    310, 320, 325, 330, 330, 325, 320, 310,
    310, 325, 325, 330, 330, 325, 325, 310,
    310, 320, 330, 330, 330, 330, 320, 310,
    310, 330, 330, 330, 330, 330, 330, 310,
    310, 325, 320, 320, 320, 320, 325, 310,
    300, 310, 310, 310, 310, 310, 310, 300,

    // Rooks
    500, 500, 500, 500, 500, 500, 500, 500,
    505, 510, 510, 510, 510, 510, 510, 505,
    495, 500, 500, 500, 500, 500, 500, 495,
    495, 500, 500, 500, 500, 500, 500, 495,
    495, 500, 500, 500, 500, 500, 500, 495,
    495, 500, 500, 500, 500, 500, 500, 495,
    495, 500, 500, 500, 500, 500, 500, 495,
    500, 500, 500, 505, 505, 500, 500, 500,

    // Queen
    880, 890, 890, 895, 895, 890, 890, 880,
    890, 900, 900, 900, 900, 900, 900, 890,
    890, 900, 905, 905, 905, 905, 900, 890,
    895, 900, 905, 905, 905, 905, 900, 895,
    900, 900, 905, 905, 905, 905, 900, 895,
    890, 905, 905, 905, 905, 905, 900, 890,
    890, 900, 905, 900, 900, 900, 900, 890,
    880, 890, 890, 895, 895, 890, 890, 880,
    // King middle game
    1170, 1160, 1160, 1150, 1150, 1160, 1160, 1170,
    1170, 1160, 1160, 1150, 1150, 1160, 1160, 1170,
    1170, 1160, 1160, 1150, 1150, 1160, 1160, 1170,
    1170, 1160, 1160, 1150, 1150, 1160, 1160, 1170,
    1180, 1170, 1170, 1160, 1160, 1170, 1170, 1180,
    1190, 1180, 1180, 1180, 1180, 1180, 1180, 1190,
    1220, 1220, 1200, 1200, 1200, 1200, 1220, 1220,
    1220, 1230, 1210, 1200, 1200, 1210, 1230, 1220};
int EvalByColor[2];

void EvalMaterial(Position *position)
{
    for (int color = COLOR_WHITE; color <= COLOR_BLACK; color++)
    {
        for (int pieceType = PAWN; pieceType <= KING; pieceType++)
        {
            U64 pieces = position->piecesByColor[color] & position->piecesByType[pieceType];
            int pieceIndex = GetPieceIndex(pieceType);
            while (pieces > 0)
            {
                int sq = __builtin_ctzll(pieces);
                int materialIndex;
                if (color == COLOR_WHITE)
                {
                    materialIndex = GetMaterialPieceIndex(pieceIndex, (sq ^ 56));
                }
                else
                {
                    materialIndex = GetMaterialPieceIndex(pieceIndex, sq);
                }

                EvalByColor[color] += MiddleGameMaterialValues[materialIndex];
                pieces &= pieces - 1;
            }
            // int numberOfPieces = __builtin_popcountll(pieces);
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