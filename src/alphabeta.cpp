#include <iostream>
#include <cstring>
#include <cassert>

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
#include "nextMove.hpp"
#include "searchstack.hpp"

int reduction(const int moveNum, const int depth)
{
    assert(moveNum >= 0);
    assert(depth >= 0);

    if(moveNum < 4 || depth < 3)
    {
        return 0;
    }
    return 1;
}

int alphaBeta(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, int alpha, int beta, int depth)
{
    assert(ss != NULL);
    assert(depth >= 0);
    assert(beta >= alpha);

    if(depth == 0 || info.depth >= MAX_DEPTH)
    {
        pv.numMoves = 0;
        info.leafNodes++;
        return eval(pos);
    }

    if(info.nodes != 0)
    {
        // Stop searching if we've ran out of time
        if(*info.stop == true || clock() >= info.end)
        {
            return 0;
        }

        // Send an update on what we're doing
        if(info.nodes % 2000000 == 0)
        {
            double timeSpent = (double)(clock() - info.start)/CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes/timeSpent)
                      << std::endl;
        }
    }

    int alphaOrig = alpha;
    uint64_t key = generateKey(pos);
    Move ttMove = NO_MOVE;

    // Check the hash table
    Entry entry = probe(info.tt, key);
    if(key == entry.key)
    {
        ttMove = getMove(entry);

#ifndef NDEBUG
        info.hashHits++;
        if(legalMove(pos, ttMove) == false)
        {
            info.hashCollisions++;
        }
#endif

        if(getDepth(entry) >= depth && legalMove(pos, ttMove) == true)
        {
            switch(getFlag(entry))
            {
                case EXACT:
                    pv.numMoves = 1;
                    pv.moves[0] = ttMove;
                    return getEval(entry);
                    break;
                case LOWERBOUND:
                    alpha = (alpha > getEval(entry) ? alpha : getEval(entry));
                    break;
                case UPPERBOUND:
                    beta = (beta < getEval(entry) ? beta : getEval(entry));
                    break;
                default:
                    assert(false);
                    break;
            }

            if(alpha >= beta)
            {
                pv.numMoves = 1;
                pv.moves[0] = ttMove;
                return getEval(entry);
            }
        }
    }

#ifdef NULLMOVE
    #define R (2)

    if(ss->nullmove && depth > 2)
    {
        PV newPV;
        newPV.numMoves = 0;

        Position newPos = pos;
        newPos.turn = !newPos.turn;

        (ss+1)->nullmove = false;
        int score = -alphaBeta(newPos, info, ss+1, newPV, -beta, -beta+1, depth-1-R);

        if(score >= beta)
        {
            return score;
        }
    }
    (ss+1)->nullmove = true;
#endif

    PV newPV;
    newPV.numMoves = 0;
    Move bestMove = NO_MOVE;
    int bestScore = -INF;
    Move moves[256];
    int numMoves = movegen(pos, moves);

    // Score moves
    int scores[256] = {0};
    for(int n = 0; n < numMoves; ++n)
    {
        if(moves[n] == ttMove)
        {
            scores[n] = 1000;
        }
#ifdef KILLER_MOVES
        else if(moves[n] == ss->killer)
        {
            scores[n] = 500;
        }
#endif
        else
        {
            scores[n] = countCaptures(pos, moves[n]);
        }

        scores[n] += (isSingle(moves[n]) ? 1 : 0);
    }

    int moveNum = 0;
    Move move = NO_MOVE;
    while(nextMove(moves, numMoves, move, scores))
    {
        assert(move != NO_MOVE);
        Position newPos = pos;

        makemove(newPos, move);

        info.nodes++;

#ifdef LMR
        int r = reduction(moveNum, depth);
        int score = -alphaBeta(newPos, info, ss+1, newPV, -alpha-1, -alpha, depth-1-r);

        // Re-search
        if(score > alpha && score < beta)
        {
            score = -alphaBeta(newPos, info, ss+1, newPV, -beta, -alpha, depth-1);
            if(score > alpha)
            {
                alpha = score;
            }
        }
#else
        int score = -alphaBeta(newPos, info, ss+1, newPV, -beta, -alpha, depth-1);
#endif

        if(score > bestScore)
        {
            bestMove = move;
            bestScore = score;
        }

        if(score > alpha)
        {
            alpha = score;

            // Update PV
            pv.moves[0] = move;
            memcpy(pv.moves + 1, newPV.moves, newPV.numMoves * sizeof(Move));
            pv.numMoves = newPV.numMoves + 1;
        }

        if(alpha >= beta)
        {
#ifdef KILLER_MOVES
            if(countCaptures(pos, move) == 0)
            {
                ss->killer = move;
            }
#endif
#ifndef NDEBUG
            info.cutoffs[moveNum] += 1;
#endif
            break;
        }

        moveNum++;
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

    uint8_t flag;
    if(bestScore <= alphaOrig)
    {
        flag = UPPERBOUND;
    }
    else if(bestScore >= beta)
    {
        flag = LOWERBOUND;
    }
    else
    {
        flag = EXACT;
    }

    add(info.tt, key, depth, bestScore, bestMove, flag);

#ifndef NDEBUG
    Entry testEntry = probe(info.tt, key);
    assert(testEntry.key == key);
    assert(testEntry.depth == depth);
    assert(testEntry.eval == bestScore);
    assert(testEntry.move == bestMove);
    assert(testEntry.flag == flag);
#endif

    return bestScore;
}