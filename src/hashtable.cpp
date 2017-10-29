#include <iostream>

#include "hashtable.hpp"

#define MAX_BYTES (1024*1024*1024)

// Entry.data item layout:
//   Search -- [8 empty] [move] [eval] [depth]
//   Perft  -- [--------nodes--------] [depth]
//
// Item sizes:
//   depth --  8 bits (256 max)
//   eval  -- 32 bits (4,294,967,296 max)
//   move  -- 16 bits (65,536 max)
//   nodes -- 56 bits (72,057,594,037,927,936 max)
//

int getDepth(const Entry& n)
{
    #ifdef HASHTABLE_PACKED
        return (n.data) & 0xFF;
    #else
        return n.depth;
    #endif
}

int getEval(const Entry& n)
{
    #ifdef HASHTABLE_PACKED
        return (n.data >> 8) & 0xFFFFFFFF;
    #else
        return n.eval;
    #endif
}

Move getMove(const Entry& n)
{
    #ifdef HASHTABLE_PACKED
        int from = (n.data >> 48) & 0xFF;
        int to = (n.data >> 40) & 0xFF;
        return (Move){.from=from, .to=to};
    #else
        return n.move;
    #endif
}

uint64_t getNodes(const Entry& n)
{
    #ifdef HASHTABLE_PACKED
        return (n.data >> 8) & 0x1FFFFFFFFFFFFFF;
    #else
        return n.nodes;
    #endif
}

void tableInit(Hashtable *table)
{
    table->numEntries = 0;
    table->maxEntries = 0;
    table->entries = NULL;
}

void printDetails(Hashtable *table)
{
    std::cout << "Num entries: " << table->numEntries << std::endl;
    std::cout << "Max entries: " << table->maxEntries << std::endl;
    std::cout << "Fullness:    " << 100.0*(double)table->numEntries/table->maxEntries << "%" << std::endl;
    std::cout << "Entry size:  " << sizeof(Entry) << std::endl;
    std::cout << "Total size:  " << sizeof(Entry)*table->maxEntries << std::endl;
}

bool clear(Hashtable *table)
{
    table->numEntries = 0;
    for(int n = 0; n < table->maxEntries; ++n)
    {
        #ifdef HASHTABLE_PACKED
            table->entries[n].key = 0ULL;
            table->entries[n].data = 0ULL;
        #else
            table->entries[n].key = 0ULL;
            table->entries[n].depth = 0;
            table->entries[n].eval = 0;
            table->entries[n].move = (Move){.from=0, .to=0};
        #endif
    }

    return true;
}

int create(Hashtable *table, const int megabytes)
{
    if(table->entries)
    {
        delete table->entries;
    }

    int bytes = megabytes * 1024 * 1024;

    if(bytes > MAX_BYTES)
    {
        bytes = MAX_BYTES;
    }

    table->numEntries = 0;
    table->maxEntries = bytes/sizeof(Entry);
    table->entries = new Entry[table->maxEntries];

    return bytes;
}

Entry probe(Hashtable *table, const uint64_t key)
{
    return table->entries[key % table->maxEntries];
}

void add(Hashtable *table, const uint64_t key, const int depth, const int eval, const Move move)
{
    int index = key % table->maxEntries;

    if(table->entries[index].key == 0ULL)
    {
        table->numEntries++;
    }

    #ifdef HASHTABLE_PACKED
        uint64_t tempDepth = (uint64_t)depth & 0xFF;
        uint64_t tempEval = ((uint64_t)eval & 0xFFFFFFFF) << 8;

        uint64_t from = (move.from & 0xFF);
        uint64_t to = (move.to & 0xFF);
        uint64_t tempMove = ((from<<8) | to) << 40;

        table->entries[index].key = key;
        table->entries[index].data = tempDepth | tempEval | tempMove;
    #else
        table->entries[index].key = key;
        table->entries[index].depth = depth;
        table->entries[index].eval = eval;
        table->entries[index].move = move;
    #endif
}

void addPerft(Hashtable *table, const uint64_t key, const int depth, const uint64_t nodes)
{
    int index = key % table->maxEntries;

    if(table->entries[index].key == 0ULL)
    {
        table->numEntries++;
    }

    #ifdef HASHTABLE_PACKED
        table->entries[index].key = key;
        table->entries[index].data = (depth & 0xFF) | ((nodes & 0x1FFFFFFFFFFFFFF) << 8);
    #else
        table->entries[index].key = key;
        table->entries[index].depth = depth;
        table->entries[index].nodes = nodes;
    #endif
}
