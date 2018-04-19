#include <iostream>
#include <climits>
#include <cassert>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"

void search(Hashtable *tt, const Position& pos, bool *stop, int depth, int movetime)
{
    assert(tt != NULL);
    assert(stop != NULL);

    uint64_t nodeTotal = 0ULL;

    // PV
    PV lastPV;
    lastPV.numMoves = 0;
    lastPV.moves[0] = NO_MOVE;

    // Search stack
    searchStack ss[MAX_DEPTH];
    for(int n = 0; n < MAX_DEPTH; ++n)
    {
        ss[n].ply = n;
#ifdef KILLER_MOVES
        ss[n].killer = NO_MOVE;
#endif
    }

    // Search info
    searchInfo info;
    info.start = clock();
    info.end = clock();
    info.nodes = 0ULL;
    info.leafNodes = 0ULL;
    info.depth = 0;
    info.selDepth = 0;
    info.stop = stop;
    info.tt = tt;
#ifndef NDEBUG
    for(int i = 0; i < 256; ++i)
    {
        info.cutoffs[i] = 0ULL;
    }
    info.hashHits = 0ULL;
    info.hashCollisions = 0ULL;
    info.singleCutoffs = 0ULL;
    info.doubleCutoffs = 0ULL;
#endif

    if(depth == 0)
    {
        depth = INT_MAX;
        info.end = info.start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        info.end = INT_MAX;
    }

    int lastScore = 0;
    for(int d = 1; d <= depth; ++d)
    {
        PV pv;
        pv.numMoves = 0;

#ifdef ASPIRATION_WINDOWS
        int score = 0;
        if(depth < 3)
        {
            score = alphaBeta(pos, info, ss, pv, -INF, INF, d);
        }
        else
        {
            for(auto r : {50, 200, INF})
            {
                int lower = lastScore - r;
                int upper = lastScore + r;
                score = alphaBeta(pos, info, ss, pv, lower, upper, d);

                if(lower < score && score < upper)
                {
                    break;
                }
            }
        }
#else
        int score = alphaBeta(pos, info, ss, pv, -INF, INF, d);
#endif
        lastScore = score;

        clock_t end = clock();
        double timeSpent = (double)(end - info.start)/CLOCKS_PER_SEC;

        // Throw away the result if we ran out of time or were asked to stop
        if(*info.stop == true || end >= info.end)
        {
            break;
        }

        nodeTotal += info.nodes;

        std::cout << "info"
                  << " depth " << d
                  << " nodes " << info.nodes
                  << " score " << score
                  << " time " << (uint64_t)(1000.0*timeSpent);

        if(pv.numMoves > 0)
        {
            std::cout << " pv";
            for(int n = 0; n < pv.numMoves; ++n)
            {
                std::cout << " " << moveString(pv.moves[n]);
            }
        }

        std::cout << std::endl;

        lastPV = pv;
    }

#ifndef NDEBUG
    std::cout << std::endl;

    uint64_t total = 0ULL;
    for(int i = 0; i < 256; ++i)
    {
        total += info.cutoffs[i];
    }
    for(int i = 0; i < 8; ++i)
    {
        std::cout << "info "
                  << "move " << i+1 << " "
                  << "cutoffs " << info.cutoffs[i] << " "
                  << "(" << 100.0*info.cutoffs[i]/total << "%)"
                  << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Single cutoffs: " << info.singleCutoffs << std::endl;
    std::cout << "Double cutoffs: " << info.doubleCutoffs << std::endl;
    std::cout << std::endl;

    std::cout << "Hash table hits: " << info.hashHits << std::endl;
    std::cout << "Collisions: " << info.hashCollisions << std::endl;
    std::cout << "Percent: " << 100.0*(double)info.hashCollisions/info.hashHits << "%" <<  std::endl;
    std::cout << std::endl;

    std::cout << "tt->numEntries: " << tt->numEntries << std::endl;
    std::cout << "tt->maxEntries: " << tt->maxEntries << std::endl;
    std::cout << std::endl;
#endif

    if(lastPV.numMoves == 0)
    {
        // If we somehow don't have a move from the PV, try play a random one
        Move moves[256];
        int numMoves = movegen(pos, moves);

        if(numMoves == 0)
        {
            std::cout << "bestmove 0000" << std::endl;
        }
        else
        {
            int n = rand() % numMoves;
            assert(legalMove(pos, moves[n]) == true);
            std::cout << "bestmove " << moveString(moves[n]) << std::endl;
        }
    }
    else
    {
        assert(legalMove(pos, lastPV.moves[0]) == true);
        std::cout << "bestmove " << moveString(lastPV.moves[0]) << std::endl;
    }
}