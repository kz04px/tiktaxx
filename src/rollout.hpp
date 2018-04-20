#ifndef ROLLOUT_HPP_INCLUDED
#define ROLLOUT_HPP_INCLUDED

#include "ataxx.hpp"

int rollout(const Position &pos, const int max_depth);
int rollout_heavy(const Position &pos, const int max_depth);

#endif
