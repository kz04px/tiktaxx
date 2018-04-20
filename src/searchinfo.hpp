#ifndef SEARCHINFO_HPP_INCLUDED
#define SEARCHINFO_HPP_INCLUDED

#include <ctime>
#include <cinttypes>
#include <unordered_map>

#include "hashtable.hpp"
#include "options.hpp"

struct searchInfo
{
    searchInfo()
    {
        nodes = 0ULL;
        leafNodes = 0ULL;
        depth = 0;
        selDepth = 0;
#ifndef NDEBUG
        for(int i = 0; i < 256; ++i)
        {
            cutoffs[i] = 0ULL;
        }
        hashHits = 0ULL;
        hashCollisions = 0ULL;
        singleCutoffs = 0ULL;
        doubleCutoffs = 0ULL;
#endif
    }

    uint64_t nodes;
    uint64_t leafNodes;
    int depth;
    int selDepth;
    clock_t start;
    clock_t end;
    bool *stop;
    Hashtable *tt;
    Options *options;
#ifndef NDEBUG
    uint64_t cutoffs[256];
    uint64_t singleCutoffs;
    uint64_t doubleCutoffs;
    uint64_t hashHits;
    uint64_t hashCollisions;
#endif
};

#endif