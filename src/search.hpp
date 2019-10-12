#ifndef SEARCH_HPP_INCLUDED
#define SEARCH_HPP_INCLUDED

#include <libataxx/position.hpp>
#include "hashtable.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "searchstack.hpp"

#define INF 1000000
#define MAX_DEPTH 128

void alphabeta(Hashtable *tt,
               Options *options,
               const libataxx::Position &pos,
               bool *stop,
               int depth,
               int movetime);

// Alpha Beta
int minimax(const libataxx::Position &pos,
            search_info &info,
            search_stack *ss,
            PV &pv,
            const int depth);
int alphabeta_search(const libataxx::Position &pos,
                     search_info &info,
                     search_stack *ss,
                     PV &pv,
                     int alpha,
                     int beta,
                     int depth);

// MCTS
void mcts_pure(const libataxx::Position &pos, int nodes, int movetime);
void mcts_uct(const libataxx::Position &pos, int nodes, int movetime);

// Other
void most_captures(const libataxx::Position &pos);
void random(const libataxx::Position &pos);

#endif
