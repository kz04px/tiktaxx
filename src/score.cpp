#include "score.hpp"
#include <cassert>
#include <libataxx/position.hpp>
#include "other.hpp"

int score(const libataxx::Position &pos) {
    int num_us = pos.get_us().count();
    int num_them = pos.get_them().count();
    [[maybe_unused]] const int num_blockers = pos.get_gaps().count();
    const int num_empty = pos.get_empty().count();

    assert(num_us + num_them + num_blockers + num_empty == 49);

    auto our_moves = pos.get_us().singles() | pos.get_us().doubles();
    our_moves &= pos.get_empty();

    auto their_moves = pos.get_them().singles() | pos.get_them().doubles();
    their_moves &= pos.get_empty();

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
