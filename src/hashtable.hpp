#ifndef HASHTABLE_HPP_INCLUDED
#define HASHTABLE_HPP_INCLUDED

#include <cstdint>

#include "move.hpp"

//#define HASHTABLE_PACKED

typedef struct
{
    uint64_t key;
    #ifdef HASHTABLE_PACKED
        uint64_t data;
    #else
        int depth;
        int eval;
        Move move;
        uint64_t nodes;
    #endif
} Entry;

typedef struct
{
    int numEntries;
    int maxEntries;
    Entry *entries;
} Hashtable;

void tableInit(Hashtable *table);
int tableCreate(Hashtable *table, int megabytes);
bool tableClear(Hashtable *table);
Entry probe(Hashtable *table, const uint64_t key);
void add(Hashtable *table, const uint64_t key, const int depth, const int eval, const Move move);
void addPerft(Hashtable *table, const uint64_t key, const int depth, const uint64_t nodes);
void printDetails(Hashtable *table);
void tableRemove(Hashtable *table);

int getDepth(const Entry& n);
int getEval(const Entry& n);
Move getMove(const Entry& n);
uint64_t getNodes(const Entry& n);

#endif
