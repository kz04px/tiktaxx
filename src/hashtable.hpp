#ifndef HASHTABLE_HPP_INCLUDED
#define HASHTABLE_HPP_INCLUDED

#include <cstdint>
#include <libataxx/move.hpp>

//#define HASHTABLE_PACKED
#define EXACT 0
#define LOWERBOUND 1
#define UPPERBOUND 2

struct Entry {
    uint64_t key;
#ifdef HASHTABLE_PACKED
    uint64_t data;
#else
    int depth;
    int eval;
    libataxx::Move move;
    uint64_t nodes;
    uint8_t flag;
#endif
};

struct Hashtable {
    int num_entries;
    int max_entries;
    Entry *entries;
};

void table_init(Hashtable *table);
int table_create(Hashtable *table, int megabytes);
bool table_clear(Hashtable *table);
Entry probe(Hashtable *table, const uint64_t key);
void add(Hashtable *table,
         const uint64_t key,
         const int depth,
         const int eval,
         const libataxx::Move &move,
         const uint8_t flag);
void add_perft(Hashtable *table,
               const uint64_t key,
               const int depth,
               const uint64_t nodes);
void print_details(Hashtable *table);
void table_remove(Hashtable *table);

int get_depth(const Entry &n);
int get_eval(const Entry &n);
libataxx::Move get_move(const Entry &n);
uint64_t get_nodes(const Entry &n);
uint8_t get_flag(const Entry &n);

int eval_to_tt(const int eval, const int ply);
int eval_from_tt(const int eval, const int ply);

#endif
