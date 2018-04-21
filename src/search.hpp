#ifndef SEARCH_HPP_INCLUDED
#define SEARCH_HPP_INCLUDED

#include "ataxx.hpp"
#include "searchinfo.hpp"
#include "hashtable.hpp"
#include "pv.hpp"
#include "searchstack.hpp"

#define INF 1000000
#define MAX_DEPTH 128

void alphabeta(Hashtable *tt, Options *options, const Position &pos, bool *stop, int depth, int movetime);

// Alpha Beta
int minimax(const Position &pos, search_info &info, search_stack *ss, PV &pv, const int depth);
int alphabeta_search(const Position &pos, search_info &info, search_stack *ss, PV &pv, int alpha, int beta, int depth);

// MCTS
void mcts_pure(const Position &pos, int num_simulations, int movetime);
void mcts_uct(const Position &pos, int num_simulations, int movetime);

// Other
void most_captures(const Position &pos);
void random(const Position &pos);

#endif
