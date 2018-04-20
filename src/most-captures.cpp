#include <iostream>
#include <vector>
#include <cassert>

#include "search.hpp"
#include "movegen.hpp"
#include "makemove.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"

void most_captures(const Position &pos)
{
    search_info info;

    Move moves[256];
    int num_moves = movegen(pos, moves);

    int most_captures = 0;
    int most_eval = -INF;
    std::vector<Move> captures;
    std::vector<Move> noncaptures;

    captures.clear();
    noncaptures.clear();

    for(int n = 0; n < num_moves; ++n)
    {
        int num_captures = count_captures(pos, moves[n]);

        info.nodes++;

        if(num_captures > 0)
        {
            if(num_captures > most_captures)
            {
                most_captures = num_captures;

                captures.clear();
                captures.push_back(moves[n]);
            }
            else if(num_captures == most_captures)
            {
                captures.push_back(moves[n]);
            }
        }
        else
        {
            Position new_pos = pos;
            makemove(new_pos, moves[n]);
            int r = -eval(new_pos);

            if(r > most_eval)
            {
                most_eval = r;

                noncaptures.clear();
                noncaptures.push_back(moves[n]);
            }
            else if(r == most_eval)
            {
                noncaptures.push_back(moves[n]);
            }
        }
    }

    if(num_moves == 0)
    {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    Move move = NO_MOVE;

    if(most_captures > 0)
    {
        int n = rand() % captures.size();
        move = captures[n];
    }
    else
    {
        int n = rand() % noncaptures.size();
        move = noncaptures[n];
    }

    std::cout << "bestmove " << move_string(move) << std::endl;
}
