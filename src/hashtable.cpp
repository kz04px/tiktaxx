#include <iostream>
#include "hashtable.hpp"

#define MAX_BYTES (1024*1024*1024)

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
        table->entries[n].key = 0ULL;
        table->entries[n].depth = 0;
        table->entries[n].eval = 0;
        table->entries[n].move = (Move){.from=0, .to=0};
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

    table->entries[index].key = key;
    table->entries[index].depth = depth;
    table->entries[index].eval = eval;
    table->entries[index].move = move;
}

