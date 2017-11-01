#ifndef SEARCH_HPP_INCLUDED
#define SEARCH_HPP_INCLUDED

#include "ataxx.hpp"
#include "searchinfo.hpp"
#include "hashtable.hpp"
#include "pv.hpp"

#define INF (1000000)
#define MAX_DEPTH (32)

void search(Hashtable *tt, const Position& pos, const int depth);
void splitSearch(Hashtable *tt, const Position& pos, const int depth);

// Alpha Beta
int minimax(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, const int depth);
int alphaBeta(const Position& pos, searchInfo& info, searchStack *ss, PV& pv, int alpha, int beta, int depth);

// MCTS
void mctsPure(const Position& pos, int numSimulations, int movetime);
void mctsUct(const Position& pos, int numSimulations, int movetime);

// Other
int mostCaptures(const Position& pos, searchInfo& info, searchStack *ss, PV& pv);
int random(const Position& pos, searchInfo& info, searchStack *ss, PV& pv);

#endif
