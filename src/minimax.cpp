#include <iostream>
#include <ctime>
#include <cstring>
#include <cassert>

#include "search.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "eval.hpp"
#include "score.hpp"

int minimax(const Position &pos, search_info &info, search_stack *ss, PV &pv, const int depth)
{
    assert(ss != NULL);

    if(depth == 0 || info.depth >= MAX_DEPTH)
    {
        pv.num_moves = 0;
        info.leaf_nodes++;
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
            double time_spent = (double)(clock() - info.start)/CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes/time_spent)
                      << std::endl;
        }
    }

    int best_score = -INF;

    PV new_pv;
    new_pv.num_moves = 0;
    Move moves[256];
    int num_moves = movegen(pos, moves);

    for(int n = 0; n < num_moves; ++n)
    {
        Position new_pos = pos;

        makemove(new_pos, moves[n]);

        info.nodes++;

        int score = -minimax(new_pos, info, ss+1, new_pv, depth-1);

        if(score > best_score)
        {
            best_score = score;
            pv.moves[0] = moves[n];
            memcpy(pv.moves + 1, new_pv.moves, new_pv.num_moves * sizeof(Move));
            pv.num_moves = new_pv.num_moves + 1;
        }
    }

    if(num_moves == 0)
    {
        pv.num_moves = 0;
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

    return best_score;
}
