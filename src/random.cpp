#include <cassert>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "eval.hpp"
#include "search.hpp"

void random(const libataxx::Position &pos) {
    libataxx::Move moves[256];
    int num_moves = pos.legal_moves(moves);

    if (num_moves == 0) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    libataxx::Position new_pos = pos;
    int n = rand() % num_moves;
    new_pos.makemove(moves[n]);

    std::cout << "info"
              << " nodes " << 1 << " score " << -eval(new_pos) << " pv "
              << moves[n] << std::endl;

    assert(pos.is_legal_move(moves[n]) == true);
    std::cout << "bestmove " << moves[n] << std::endl;
}
