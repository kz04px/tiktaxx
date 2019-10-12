#include "score.hpp"
#include <cassert>
#include <libataxx/position.hpp>
#include "other.hpp"

int score(const libataxx::Position &pos) {
    int num_us = pos.us().count();
    int num_them = pos.them().count();
    const int num_blockers = pos.gaps().count();
    const int num_empty = pos.empty().count();

    assert(num_us + num_them + num_blockers + num_empty == 49);

    auto our_moves = pos.us().singles() | pos.us().doubles();
    our_moves &= pos.empty();

    auto their_moves = pos.them().singles() | pos.them().doubles();
    their_moves &= pos.empty();

    if (our_moves == 0ULL && their_moves != 0ULL) {
        num_them += num_empty;
    } else if (our_moves != 0ULL && their_moves == 0ULL) {
        num_us += num_empty;
    }

    const int score = num_us - num_them;

    assert(-49 + num_blockers <= score);
    assert(score <= 49 - num_blockers);

    return score;
}
