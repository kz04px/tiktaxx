#ifndef ROLLOUT_HPP_INCLUDED
#define ROLLOUT_HPP_INCLUDED

#include "ataxx.hpp"

float rollout(const Position &pos, const int max_depth);
float rollout_heavy(const Position &pos, const int max_depth);

#endif
