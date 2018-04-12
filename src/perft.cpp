#include <iostream>
#include <ctime>

#include "perft.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "zobrist.hpp"

uint64_t perftSearch(Hashtable *tt, const Position& pos, const int depth)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    if(depth == 1)
    {
        return numMoves;
    }

    uint64_t key = generateKey(pos);

    // Check the hash table
    Entry entry = probe(tt, key);
    if(key == entry.key && getDepth(entry) == depth)
    {
        return getNodes(entry);
    }

    uint64_t nodes = 0ULL;

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        nodes += perftSearch(tt, newPos, depth-1);
    }

    // Add the hash table entry
    addPerft(tt, key, depth, nodes);

    return nodes;
}

void splitPerft(Hashtable *tt, const Position& pos, const int depth)
{
    tableClear(tt);
    uint64_t total = 0ULL;
    Move moves[256];
    int numMoves = movegen(pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        std::cout << moveString(moves[n])
                  << " "
                  << (moves[n].to == moves[n].from ? "single" : "double")
                  << "   ";

        uint64_t nodes = perftSearch(tt, newPos, depth-1);

        std::cout << nodes
                  << std::endl;
        total += nodes;
    }
    tableClear(tt);
    std::cout << "nodes " << total << std::endl;
}

void perft(Hashtable *tt, const Position& pos, const int depth)
{
    tableClear(tt);
    uint64_t nodes = 0ULL;
    for(int n = 1; n <= depth; ++n)
    {
        clock_t start = clock();
        nodes = perftSearch(tt, pos, n);
        clock_t end = clock();
        double timeSpent = (double)(end-start)/CLOCKS_PER_SEC;

        std::cout << "info";
        std::cout << " depth " << n;
        std::cout << " nodes " << nodes;
        std::cout << " time " << 1000.0*timeSpent;
        if(timeSpent > 0.0)
        {
            std::cout << " nps " << (uint64_t)(nodes/timeSpent);
        }
        std::cout << std::endl;
    }
    std::cout << "nodes " << nodes << std::endl;
}