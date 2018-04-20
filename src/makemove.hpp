#ifndef MAKEMOVE_HPP_INCLUDED
#define MAKEMOVE_HPP_INCLUDED

#include <string>

#include "ataxx.hpp"
#include "move.hpp"

void makemove(Position &pos, const Move &n);
void makemove(Position &pos, const int to);
void makemove(Position &pos, const int from, const int to);
void makemove(Position &pos, const std::string &move_string);
bool makemoves(Position &pos, const std::string &moves_string);

#endif
