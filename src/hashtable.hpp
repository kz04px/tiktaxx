#ifndef HASHTABLE_HPP_INCLUDED
#define HASHTABLE_HPP_INCLUDED

#include <cstdint>
#include "move.hpp"

typedef struct
{
    uint64_t key;
    int depth;
    Move move;
    int eval;
} Entry;

typedef struct
{
    int numEntries;
    int maxEntries;
    Entry *entries;
} Hashtable;

void tableInit(Hashtable *table);
int create(Hashtable *table, int megabytes);
bool clear(Hashtable *table);
Entry probe(Hashtable *table, const uint64_t key);
void add(Hashtable *table, const uint64_t key, const int depth, const int eval, const Move move);
void printDetails(Hashtable *table);

#endif
