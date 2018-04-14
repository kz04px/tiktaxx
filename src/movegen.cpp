#include <cassert>

#include "movegen.hpp"
#include "bitboards.hpp"
#include "other.hpp"

int movegen(const Position& pos, Move *moves)
{
    assert(moves != NULL);

    int numMoves = 0;

    // Create double moves
    uint64_t piecesCopy = pos.pieces[pos.turn];
    while(piecesCopy)
    {
        int from = lsb(piecesCopy);
        uint64_t fromBB = (1ULL)<<from;

        uint64_t toBB = doubleJump(fromBB);

        // Can't move onto friendly pos.pieces
        toBB &= ~pos.pieces[pos.turn];

        // Can't move onto unfriendly pos.pieces
        toBB &= ~pos.pieces[!pos.turn];

        // Can't move onto blockers
        toBB &= ~pos.blockers;

        while(toBB)
        {
            int to = lsb(toBB);

            moves[numMoves] = (Move){.from=from, .to=to};
            numMoves++;

            toBB &= toBB - 1;
        }

        piecesCopy &= piecesCopy - 1;
    }

    uint64_t singles = singleJump(pos.pieces[pos.turn]);

    // Can't move onto friendly pos.pieces
    singles &= ~pos.pieces[pos.turn];

    // Can't move onto unfriendly pos.pieces
    singles &= ~pos.pieces[!pos.turn];

    // Can't move onto blockers
    singles &= ~pos.blockers;

    // Create single moves
    while(singles)
    {
        int to = lsb(singles);

        moves[numMoves] = (Move){.from=to, .to=to};
        numMoves++;

        singles &= singles - 1;
    }

    assert(numMoves >= 0);
    assert(numMoves < 256);

#ifndef NDEBUG
    for(int i = 0; i < numMoves; ++i)
    {
        // Either one or the other
        assert((isSingle(moves[i]) ^ isDouble(moves[i])) != 0);

        // Destination needs to be empty
        assert(((pos.pieces[pos.turn] | pos.pieces[!pos.turn] | pos.blockers) & ((1ULL)<<moves[i].to)) == 0ULL);

        if(isDouble(moves[i]) == true)
        {
            // Source needs to have our stone on it
            assert((pos.pieces[pos.turn] & ((1ULL)<<moves[i].from)) != 0ULL);
        }
    }
#endif

    return numMoves;
}