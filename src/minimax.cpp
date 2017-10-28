#include <iostream>
#include <ctime>

#include "search.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "eval.hpp"
#include "score.hpp"

int minimax(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, const int depth)
{
    if(depth == 0 || info.depth >= MAX_DEPTH)
    {
        pv.numMoves = 0;
        info.leafNodes++;
        return eval(pos);
    }

    if(info.nodes != 0)
    {
        // Stop searching if we've ran out of time
        if(clock() >= info.end)
        {
            return 0;
        }

        // Send an update on what we're doing
        if(info.nodes % 500000 == 0)
        {
            double timeSpent = (double)(clock() - info.start)/CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes/timeSpent)
                      << std::endl;
        }
    }

    int bestScore = -INF;

    PV newPV;
    newPV.numMoves = 0;
    Move moves[256];
    int numMoves = movegen(pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        info.nodes++;

        int score = -minimax(newPos, info, ss+1, newPV, depth-1);

        if(score > bestScore)
        {
            bestScore = score;
            pv.moves[0] = moves[n];
            memcpy(pv.moves + 1, newPV.moves, newPV.numMoves * sizeof(Move));
            pv.numMoves = newPV.numMoves + 1;
        }
    }

    if(numMoves == 0)
    {
        pv.numMoves = 0;
        int val = score(pos);

        if(val > 0)
        {
            return INF - ss->ply;
        }
        else if(val < 0)
        {
            return -INF + ss->ply;
        }
        else
        {
            return 0;
        }
    }

    return bestScore;
}
