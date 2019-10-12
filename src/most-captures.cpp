#include <cassert>
#include <iostream>
#include <libataxx/position.hpp>
#include <vector>
#include "eval.hpp"
#include "other.hpp"
#include "search.hpp"
#include "searchinfo.hpp"

void most_captures(const libataxx::Position &pos) {
    search_info info;

    libataxx::Move moves[256];
    int num_moves = pos.legal_moves(moves);

    int most_captures = 0;
    int most_eval = -INF;
    std::vector<libataxx::Move> captures;
    std::vector<libataxx::Move> noncaptures;

    captures.clear();
    noncaptures.clear();

    for (int n = 0; n < num_moves; ++n) {
        int num_captures = count_captures(pos, moves[n]);

        info.nodes++;

        if (num_captures > 0) {
            if (num_captures > most_captures) {
                most_captures = num_captures;

                captures.clear();
                captures.push_back(moves[n]);
            } else if (num_captures == most_captures) {
                captures.push_back(moves[n]);
            }
        } else {
            libataxx::Position new_pos = pos;
            new_pos.makemove(moves[n]);
            int r = -eval(new_pos);

            if (r > most_eval) {
                most_eval = r;

                noncaptures.clear();
                noncaptures.push_back(moves[n]);
            } else if (r == most_eval) {
                noncaptures.push_back(moves[n]);
            }
        }
    }

    if (num_moves == 0) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    libataxx::Move move = libataxx::Move::nullmove();

    if (most_captures > 0) {
        int n = rand() % captures.size();
        move = captures[n];
    } else {
        int n = rand() % noncaptures.size();
        move = noncaptures[n];
    }

    std::cout << "bestmove " << move << std::endl;
}
