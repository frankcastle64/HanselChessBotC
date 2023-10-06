#include <stdio.h>
#include "pieces.h"
#include "fen_manager.h"
#include "init.h"
#include "display.h"
#include <inttypes.h>
#include "move_gen.h"
#include "perft_manager.h"
#include "magic_bitboards.h"
#include <sys/time.h>
#include "search.h"
#include "eval.h"
// #include "tests.h"
#include "string.h"
#include "uci_manager.h"

// long long getCurrentTimeInMilliseconds()
// {
//     struct timeval currentTime;
//     gettimeofday(&currentTime, NULL);
//     return currentTime.tv_sec * 1000LL + currentTime.tv_usec / 1000LL;
// }

long long getCurrentTimeInMilliseconds1()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000LL + currentTime.tv_usec / 1000LL;
}

void Trial(int depth, char *fen)
{
    // int depth = 6;
    // char *fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

    BoardState boardState[1];
    InitAll(boardState);
    SetBoard(fen, boardState);
    long long t0 = getCurrentTimeInMilliseconds1();
    U64 nodes = Perft(boardState->position, depth, 0);
    long long t1 = getCurrentTimeInMilliseconds1();
    printf("nodes:%lld", nodes);

    // printf("number of nodes %lld\ttime in ms %lld\n", nodes, (t1 - t0));
}
/*
void Trial1(){
     char str[17]; // 16 characters plus null terminator
        snprintf(str, sizeof(str), "%016" "llX", rankbb);
        printf("0x%sULL;\n", str);

}*/

void EnginePlay(char *fen, int depth)
{
    // char *fen = "8/3p1p2/5Ppp/K2R2bk/4pPrr/6Pp/4B2P/3N4 w - - 0 1";
    BoardState boardState[1];
    InitAll(boardState);
    SetBoard(fen, boardState);
    Think(boardState->position, depth);
}
int main(int argc, char **argv)
{
    
    UciLoop();
    return 0;
}
