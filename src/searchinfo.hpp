#ifndef SEARCHINFO_HPP_INCLUDED
#define SEARCHINFO_HPP_INCLUDED

#include <ctime>
#include <cinttypes>

#include "hashtable.hpp"
#include "options.hpp"

struct search_info
{
    search_info()
    {
        nodes = 0ULL;
        leaf_nodes = 0ULL;
        depth = 0;
        sel_depth = 0;
        start = 0;
        end = 0;
        stop = NULL;
        tt = NULL;
        options = NULL;
#ifndef NDEBUG
        for(int i = 0; i < 256; ++i)
        {
            cutoffs[i] = 0ULL;
        }
        hash_hits = 0ULL;
        hash_collisions = 0ULL;
        single_cutoffs = 0ULL;
        double_cutoffs = 0ULL;
#endif
    }

    uint64_t nodes;
    uint64_t leaf_nodes;
    int depth;
    int sel_depth;
    clock_t start;
    clock_t end;
    bool *stop;
    Hashtable *tt;
    Options *options;
#ifndef NDEBUG
    uint64_t cutoffs[256];
    uint64_t single_cutoffs;
    uint64_t double_cutoffs;
    uint64_t hash_hits;
    uint64_t hash_collisions;
#endif
};

#endif
