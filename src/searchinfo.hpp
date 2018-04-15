#ifndef SEARCHINFO_HPP_INCLUDED
#define SEARCHINFO_HPP_INCLUDED

#include <ctime>
#include <cinttypes>

#include "hashtable.hpp"

typedef struct
{
    uint64_t nodes;
    uint64_t leafNodes;
    int depth;
    int selDepth;
    clock_t start;
    clock_t end;
    bool *stop;
    Hashtable *tt;
#ifndef NDEBUG
    uint64_t cutoffs[256];
#endif
} searchInfo;

#endif