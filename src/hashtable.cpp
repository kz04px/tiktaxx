#include "hashtable.hpp"
#include <cassert>
#include <iostream>
#include <libataxx/move.hpp>

#define MAX_MEGABYTES (4 * 1024)
#define INF 1000000
#define MAX_DEPTH 128

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

int get_depth(const Entry &n) {
#ifdef HASHTABLE_PACKED
    return (n.data) & 0xFF;
#else
    return n.depth;
#endif
}

int get_eval(const Entry &n) {
#ifdef HASHTABLE_PACKED
    return (n.data >> 8) & 0xFFFFFFFF;
#else
    return n.eval;
#endif
}

libataxx::Move get_move(const Entry &n) {
#ifdef HASHTABLE_PACKED
    int from = (n.data >> 48) & 0xFF;
    int to = (n.data >> 40) & 0xFF;
    return (libataxx::Move){.from = from, .to = to};
#else
    return n.move;
#endif
}

uint64_t get_nodes(const Entry &n) {
#ifdef HASHTABLE_PACKED
    return (n.data >> 8) & 0x1FFFFFFFFFFFFFF;
#else
    return n.nodes;
#endif
}

uint8_t get_flag(const Entry &n) {
#ifdef HASHTABLE_PACKED
    // TO DO:
    // Implement this
    assert(false);
    return EXACT;
#else
    return n.flag;
#endif
}

void table_init(Hashtable *table) {
    assert(table != NULL);

    table->num_entries = 0;
    table->max_entries = 0;
    table->entries = NULL;
}

void print_details(Hashtable *table) {
    assert(table != NULL);

    std::cout << "Num entries: " << table->num_entries << std::endl;
    std::cout << "Max entries: " << table->max_entries << std::endl;
    std::cout << "Entry size:  " << sizeof(Entry) << std::endl;
    if (table->max_entries > 0) {
        std::cout << "Fullness:    "
                  << 100.0 * (double)table->num_entries / table->max_entries
                  << "%" << std::endl;
    }
    std::cout << "Total size:  " << sizeof(Entry) * table->max_entries
              << std::endl;
}

bool table_clear(Hashtable *table) {
    assert(table != NULL);

    table->num_entries = 0;
    for (int n = 0; n < table->max_entries; ++n) {
#ifdef HASHTABLE_PACKED
        table->entries[n].key = 0ULL;
        table->entries[n].data = 0ULL;
#else
        table->entries[n].key = 0ULL;
        table->entries[n].depth = 0;
        table->entries[n].eval = 0;
        table->entries[n].move = libataxx::Move::nullmove();
        table->entries[n].flag = EXACT;
#endif
    }

    return true;
}

int table_create(Hashtable *table, int megabytes) {
    assert(table != NULL);

    if (table->entries) {
        table_remove(table);
    }

    if (megabytes > MAX_MEGABYTES) {
        megabytes = MAX_MEGABYTES;
    } else if (megabytes < 1) {
        megabytes = 1;
    }

    table->num_entries = 0;

    while (megabytes > 0) {
        uint64_t bytes = megabytes * 1024 * 1024;
        table->max_entries = bytes / sizeof(Entry);

        try {
            table->entries = new Entry[table->max_entries];
            break;
        } catch (...) {
            megabytes = megabytes >> 1;
        }
    }

    return megabytes;
}

void table_remove(Hashtable *table) {
    assert(table != NULL);

    if (table->entries) {
        table->num_entries = 0;
        table->max_entries = 0;
        delete table->entries;
    }
}

Entry probe(Hashtable *table, const uint64_t key) {
    assert(table != NULL);

    return table->entries[key % table->max_entries];
}

void add(Hashtable *table,
         const uint64_t key,
         const int depth,
         const int eval,
         const libataxx::Move &move,
         const uint8_t flag) {
    assert(table != NULL);
    assert(eval <= INF);
    assert(eval >= -INF);

    const int index = key % table->max_entries;

    if (table->entries[index].key == 0ULL) {
        table->num_entries++;
    }

#ifdef HASHTABLE_PACKED
    uint64_t temp_depth = (uint64_t)depth & 0xFF;
    uint64_t temp_eval = ((uint64_t)eval & 0xFFFFFFFF) << 8;

    uint64_t from = (move.from & 0xFF);
    uint64_t to = (move.to & 0xFF);
    uint64_t temp_move = ((from << 8) | to) << 40;

    table->entries[index].key = key;
    table->entries[index].data = temp_depth | temp_eval | temp_move;
#else
    table->entries[index].key = key;
    table->entries[index].depth = depth;
    table->entries[index].eval = eval;
    table->entries[index].move = move;
    table->entries[index].flag = flag;
#endif
}

void add_perft(Hashtable *table,
               const uint64_t key,
               const int depth,
               const uint64_t nodes) {
    assert(table != NULL);

    const int index = key % table->max_entries;

    if (table->entries[index].key == 0ULL) {
        table->num_entries++;
    }

#ifdef HASHTABLE_PACKED
    table->entries[index].key = key;
    table->entries[index].data =
        (depth & 0xFF) | ((nodes & 0x1FFFFFFFFFFFFFF) << 8);
#else
    table->entries[index].key = key;
    table->entries[index].depth = depth;
    table->entries[index].nodes = nodes;
#endif
}

int eval_to_tt(const int eval, const int ply) {
    assert(ply <= MAX_DEPTH);
    assert(eval <= INF);
    assert(eval >= -INF);
    if (eval > INF - MAX_DEPTH) {
        return eval + ply;
    }
    if (eval < -INF + MAX_DEPTH) {
        return eval - ply;
    }
    return eval;
}

int eval_from_tt(const int eval, const int ply) {
    assert(ply <= MAX_DEPTH);
    assert(eval <= INF);
    assert(eval >= -INF);
    if (eval > INF - MAX_DEPTH) {
        return eval - ply;
    }
    if (eval < -INF + MAX_DEPTH) {
        return eval + ply;
    }
    return eval;
}
