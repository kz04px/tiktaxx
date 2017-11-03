#include <iostream>
#include <vector>

#include "search.hpp"
#include "movegen.hpp"
#include "makemove.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"

int mostCaptures(const Position& pos, searchInfo& info, searchStack *ss, PV& pv)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    int mostCaptures = 0;
    int mostEval = -INF;
    std::vector<Move> captures;
    std::vector<Move> noncaptures;

    captures.clear();
    noncaptures.clear();

    for(int n = 0; n < numMoves; ++n)
    {
        int numCaptures = countCaptures(pos, moves[n]);

        info.nodes++;

        if(numCaptures > 0)
        {
            if(numCaptures > mostCaptures)
            {
                mostCaptures = numCaptures;

                captures.clear();
                captures.push_back(moves[n]);
            }
            else if(numCaptures == mostCaptures)
            {
                captures.push_back(moves[n]);
            }
        }
        else
        {
            Position newPos = pos;
            makemove(newPos, moves[n]);
            int r = -eval(newPos);

            if(r > mostEval)
            {
                mostEval = r;

                noncaptures.clear();
                noncaptures.push_back(moves[n]);
            }
            else if(r == mostEval)
            {
                noncaptures.push_back(moves[n]);
            }
        }
    }

    if(numMoves == 0)
    {
        pv.numMoves = 0;
        return 0;
    }

    if(mostCaptures > 0)
    {
        int n = rand() % captures.size();
        pv.numMoves = 1;
        pv.moves[0] = captures[n];

        Position newPos = pos;
        makemove(newPos, captures[n]);
        return -eval(newPos);
    }
    else
    {
        int n = rand() % noncaptures.size();
        pv.numMoves = 1;
        pv.moves[0] = noncaptures[n];

        return mostEval;
    }
}
