#ifndef SORTING_HPP_INCLUDED
#define SORTING_HPP_INCLUDED

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>

void sort_moves(const libataxx::Position &pos,
                libataxx::Move *moves,
                const int num_moves,
                const libataxx::Move &tt_move);

#endif
