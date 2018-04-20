#ifndef SEARCH_HPP_INCLUDED
#define SEARCH_HPP_INCLUDED

#include "ataxx.hpp"
#include "searchinfo.hpp"
#include "hashtable.hpp"
#include "pv.hpp"
#include "searchstack.hpp"

#define INF (1000000)
#define MAX_DEPTH (32)

void alphabeta(Hashtable *tt, Options *options, const Position& pos, bool *stop, int depth, int movetime);

// Alpha Beta
int minimax(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, const int depth);
int alphabetaSearch(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, int alpha, int beta, int depth);

// MCTS
void mctsPure(const Position& pos, int numSimulations, int movetime);
void mctsUCT(const Position& pos, int numSimulations, int movetime);

// Other
void mostCaptures(const Position& pos);
void random(const Position& pos);

#endif