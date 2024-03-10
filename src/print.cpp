#include "print.hpp"
#include <iostream>
#include <libataxx/bitboard.hpp>
#include <libataxx/file.hpp>
#include <libataxx/rank.hpp>
#include <libataxx/square.hpp>
#include "eval.hpp"
#include "phase.hpp"
#include "score.hpp"

void print(const libataxx::Position &pos, bool details) {
    std::cout << "  a b c d e f g" << std::endl;
    std::cout << " ╔═╦═╦═╦═╦═╦═╦═╗" << std::endl;
    for (int y = 6; y >= 0; --y) {
        std::cout << y + 1 << "║";
        for (int x = 0; x < 7; ++x) {
            const auto f = libataxx::File{x};
            const auto r = libataxx::Rank{y};
            const auto sq = libataxx::Square{f, r};
            const auto bb = libataxx::Bitboard{sq};

            if (pos.get_black() & bb) {
                std::cout << "X";
            } else if (pos.get_white() & bb) {
                std::cout << "O";
            } else if (pos.get_gaps() & bb) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
            std::cout << "║";
        }
        std::cout << y + 1 << std::endl;
        if (y > 0) {
            std::cout << " ╠═╬═╬═╬═╬═╬═╬═╣" << std::endl;
        }
    }
    std::cout << " ╚═╩═╩═╩═╩═╩═╩═╝" << std::endl;
    std::cout << "  a b c d e f g" << std::endl;

    if (details == true) {
        std::cout << "Turn: "
                  << (pos.get_turn() == libataxx::Side::Black ? 'X' : 'O')
                  << std::endl;
        std::cout << "Eval: " << eval(pos) << std::endl;
        std::cout << "Score: " << score(pos) << std::endl;
        std::cout << "Phase: " << phase(pos) << std::endl;
        std::cout << "Endgame: " << (is_endgame(pos) == true ? "true" : "false")
                  << std::endl;
    }
}
