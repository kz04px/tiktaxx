#include "other.hpp"

int count_captures(const libataxx::Position &pos, const libataxx::Move &move) {
    const auto captured =
        libataxx::Bitboard{move.to()}.singles() & pos.get_them();
    return captured.count();
}
