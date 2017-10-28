#include "ataxx.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"
#include "score.hpp"

int random(const Position& pos, searchInfo& info, searchStack *ss, PV& pv)
{
    pv.numMoves = 0;
    Move moves[256];
    int numMoves = movegen(pos, moves);
    info.nodes++;

    if(numMoves > 0)
    {
        int n = rand()%numMoves;
        pv.numMoves = 1;
        pv.moves[0] = moves[n];
        Position newPos = pos;

        makemove(newPos, moves[n]);
        return -eval(newPos);
    }

    return score(pos);
}
