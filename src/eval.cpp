#include "eval.hpp"
#include <iostream>
#include <libataxx/bitboard.hpp>
#include "other.hpp"
#include "phase.hpp"
#include "score.hpp"

#define INF 1000000
#define TURN_BONUS 300
#define PIECE_VALUE 100
#define MOBILITY_VALUE 3
#define PST_MUL 3

const int PST[49] = {30, 20, 10, 10, 10, 20, 30,
                     20, 10, 10, 5,  10, 10, 20,
                     10, 10, 5,  0,  5,  10, 10,
                     10, 5,  0,  0,  0,   5, 10,
                     10, 10, 5,  0,  5,  10, 10,
                     20, 10, 10, 5, 10,  10, 20,
                     30, 20, 10, 10, 10, 20, 30};

void split_eval(const libataxx::Position &pos) {
    int num_friendly = pos.us().count();
    int num_unfriendly = pos.them().count();

    std::cout << "Num friendly:   " << num_friendly << std::endl;
    std::cout << "Num unfriendly: " << num_unfriendly << std::endl;
}

int eval(const libataxx::Position &pos) {
    int num_friendly = pos.us().count();
    int num_unfriendly = pos.them().count();
    int our_mobility = 0;
    int their_mobility = 0;
    const auto empty = pos.empty();
    float p = phase(pos);

    auto our_moves = pos.us().singles() | pos.us().doubles();
    our_moves &= empty;

    auto their_moves = pos.them().singles() | pos.them().doubles();
    their_moves &= empty;

    int our_pst = 0;
    int their_pst = 0;

    for (const auto &sq : pos.us()) {
        our_pst += PST[static_cast<int>(sq)] * PST_MUL;
    }

    for (const auto &sq : pos.them()) {
        their_pst += PST[static_cast<int>(sq)] * PST_MUL;
    }

    int score = (p * TURN_BONUS + 200) + PIECE_VALUE * num_friendly -
                PIECE_VALUE * num_unfriendly + MOBILITY_VALUE * our_mobility -
                MOBILITY_VALUE * their_mobility + our_pst - their_pst;

    return score;
}
