#include <cassert>
#include "search.hpp"
#include "ataxx.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "eval.hpp"

void random(const Position &pos)
{
    Move moves[256];
    int num_moves = movegen(pos, moves);

    if(num_moves == 0)
    {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    Position new_pos = pos;
    int n = rand()%num_moves;
    makemove(new_pos, moves[n]);

    std::cout << "info"
              << " nodes " << 1
              << " score " << -eval(new_pos)
              << " pv " << move_string(moves[n])
              << std::endl;

    assert(legal_move(pos, moves[n]) == true);
    std::cout << "bestmove " << move_string(moves[n]) << std::endl;
}
