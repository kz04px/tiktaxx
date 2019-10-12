#ifndef OTHER_HPP_INCLUDED
#define OTHER_HPP_INCLUDED

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);

int count_captures(const libataxx::Position &pos, const libataxx::Move &move);

#endif
