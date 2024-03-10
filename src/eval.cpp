#include "eval.hpp"
#include <iostream>
#include <libataxx/bitboard.hpp>
#include "other.hpp"
#include "phase.hpp"
#include "score.hpp"

#define INF 1000000
#define TURN_BONUS 300
#define PIECE_VALUE 100
#define MOBILITY_VALUE 1

const int PST[49] = {30, 20, 10, 10, 10, 20, 30, 20, 10, 10, 5,  10, 10,
                     20, 10, 10, 5,  0,  5,  10, 10, 10, 5,  0,  0,  0,
                     5,  10, 10, 10, 5,  0,  5,  10, 10, 20, 10, 10, 5,
                     10, 10, 20, 30, 20, 10, 10, 10, 20, 30};

void split_eval(const libataxx::Position &pos) {
    int num_friendly = pos.get_us().count();
    int num_unfriendly = pos.get_them().count();

    std::cout << "Num friendly:   " << num_friendly << std::endl;
    std::cout << "Num unfriendly: " << num_unfriendly << std::endl;
}

int eval(const libataxx::Position &pos) {
    int num_friendly = pos.get_us().count();
    int num_unfriendly = pos.get_them().count();
    int our_mobility = 0;
    int their_mobility = 0;
    const auto empty = pos.get_empty();
    float p = phase(pos);

    auto our_moves = pos.get_us().singles() | pos.get_us().doubles();
    our_moves &= empty;

    auto their_moves = pos.get_them().singles() | pos.get_them().doubles();
    their_moves &= empty;

    int our_pst = 0;
    int their_pst = 0;

    for (const auto &sq : pos.get_us()) {
        our_pst += PST[sq.index()];
    }

    for (const auto &sq : pos.get_them()) {
        their_pst += PST[sq.index()];
    }

    int score = (p * TURN_BONUS + 200) + PIECE_VALUE * num_friendly -
                PIECE_VALUE * num_unfriendly + MOBILITY_VALUE * our_mobility -
                MOBILITY_VALUE * their_mobility + our_pst - their_pst;

    return score;
}
