#include <iostream>
#include <ctime>

#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "hashtable.hpp"
#include "zobrist.hpp"

uint64_t perftSearch(const Position& pos, const int depth)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    if(depth == 1)
    {
        return numMoves;
    }
/*
    uint64_t key = hash.generate(pos);

    // Check the hash table
    Entry entry = tt->probe(key);
    if(key == entry.key && entry.depth == depth)
    {
        return entry.nodes;
    }
*/
    uint64_t nodes = 0ULL;

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        nodes += perftSearch(newPos, depth-1);
    }

    // Add the hash table entry
    //tt->add(key, depth, nodes);

    return nodes;
}

void splitPerft(const Position& pos, const int depth)
{
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

        uint64_t nodes = perftSearch(newPos, depth-1);

        std::cout << nodes
                  << std::endl;
    }
}

void perft(const Position& pos, const int depth)
{
    for(int n = 1; n <= depth; ++n)
    {
        clock_t start = clock();
        uint64_t nodes = perftSearch(pos, n);
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
}
