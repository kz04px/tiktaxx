#ifndef NEXT_MOVE_HPP_INCLUDED
#define NEXT_MOVE_HPP_INCLUDED

#include <libataxx/move.hpp>

bool next_move(libataxx::Move *moves,
               const int num_moves,
               libataxx::Move &move,
               int *scores, const int move_num);

#endif
