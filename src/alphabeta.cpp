#include <iostream>

#include "ataxx.hpp"
#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "hashtable.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"
#include "score.hpp"
#include "zobrist.hpp"
#include "sorting.hpp"

int alphaBeta(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, int alpha, int beta, int depth)
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

    uint64_t key = generateKey(pos);
    Move ttMove = NO_MOVE;

    // Check the hash table
    Entry entry = probe(info.tt, key);
    if(key == entry.key)
    {
        ttMove = getMove(entry);

        if(getDepth(entry) >= depth)
        {
            pv.numMoves = 1;
            pv.moves[0] = ttMove;
            return getEval(entry);
        }
    }

    PV newPV;
    newPV.numMoves = 0;
    Move bestMove = NO_MOVE;
    Move moves[256];
    int numMoves = movegen(pos, moves);

    // Sort moves
    sortMoves(pos, moves, numMoves, ttMove);

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        info.nodes++;

        int score = -alphaBeta(newPos, info, ss+1, newPV, -beta, -alpha, depth-1);

        if(score >= beta)
        {
           return beta;
        }

        if(score > alpha)
        {
            alpha = score;

            bestMove = moves[n];

            // Update PV
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

    // Add the hash table entry
    add(info.tt, key, depth, alpha, bestMove);

    return alpha;
}
