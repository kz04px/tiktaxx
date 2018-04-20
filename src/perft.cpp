#include <iostream>
#include <ctime>
#include <cassert>

#include "perft.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "zobrist.hpp"

uint64_t perft_search(Hashtable *tt, const Position &pos, const int depth)
{
    assert(tt != NULL);

    Move moves[256];
    int num_moves = movegen(pos, moves);

    if(depth == 1)
    {
        return num_moves;
    }

    uint64_t key = generate_key(pos);

    // Check the hash table
    Entry entry = probe(tt, key);
    if(key == entry.key && get_depth(entry) == depth)
    {
        return get_nodes(entry);
    }

    uint64_t nodes = 0ULL;

    for(int n = 0; n < num_moves; ++n)
    {
        Position new_pos = pos;

        makemove(new_pos, moves[n]);

        nodes += perft_search(tt, new_pos, depth-1);
    }

    // Add the hash table entry
    add_perft(tt, key, depth, nodes);

    return nodes;
}

void split_perft(Hashtable *tt, const Position &pos, const int depth)
{
    assert(tt != NULL);

    table_clear(tt);
    uint64_t total = 0ULL;
    Move moves[256];
    int num_moves = movegen(pos, moves);

    for(int n = 0; n < num_moves; ++n)
    {
        Position new_pos = pos;

        makemove(new_pos, moves[n]);

        std::cout << move_string(moves[n])
                  << " "
                  << (moves[n].to == moves[n].from ? "single" : "double")
                  << "   ";

        uint64_t nodes = perft_search(tt, new_pos, depth-1);

        std::cout << nodes
                  << std::endl;
        total += nodes;
    }
    table_clear(tt);
    std::cout << "nodes " << total << std::endl;
}

void perft(Hashtable *tt, const Position &pos, const int depth)
{
    assert(tt != NULL);

    table_clear(tt);
    uint64_t nodes = 0ULL;
    for(int n = 1; n <= depth; ++n)
    {
        clock_t start = clock();
        nodes = perft_search(tt, pos, n);
        clock_t end = clock();
        double time_spent = (double)(end-start)/CLOCKS_PER_SEC;

        std::cout << "info";
        std::cout << " depth " << n;
        std::cout << " nodes " << nodes;
        std::cout << " time " << 1000.0*time_spent;
        if(time_spent > 0.0)
        {
            std::cout << " nps " << (uint64_t)(nodes/time_spent);
        }
        std::cout << std::endl;
    }
    std::cout << "nodes " << nodes << std::endl;
}
