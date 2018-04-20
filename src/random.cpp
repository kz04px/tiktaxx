#include <cassert>

#include "search.hpp"
#include "ataxx.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"
#include "score.hpp"
#include "searchstack.hpp"

void random(const Position &pos)
{
    Move move = NO_MOVE;
    int score = 0;
    Move moves[256];
    int num_moves = movegen(pos, moves);

    if(num_moves > 0)
    {
        Position new_pos = pos;

        int n = rand()%num_moves;
        move = moves[n];

        makemove(new_pos, move);
        score = -eval(new_pos);
    }

    std::cout << "info"
              << " nodes " << 1
              << " score " << score
              << std::endl;

    assert(legal_move(pos, move) == true);
    std::cout << "bestmove " << move_string(move) << std::endl;
}
