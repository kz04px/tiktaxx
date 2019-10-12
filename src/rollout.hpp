#ifndef ROLLOUT_HPP_INCLUDED
#define ROLLOUT_HPP_INCLUDED

#include <libataxx/position.hpp>

float rollout(const libataxx::Position &pos, const int max_depth);
float rollout_heavy(const libataxx::Position &pos, const int max_depth);

#endif
