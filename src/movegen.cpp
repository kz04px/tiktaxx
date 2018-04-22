#include <cassert>

#include "movegen.hpp"
#include "bitboards.hpp"
#include "other.hpp"

int movegen(const Position &pos, Move *moves)
{
    assert(moves != NULL);

    int num_moves = 0;

    // Create double moves
    uint64_t copy = pos.pieces[pos.turn];
    while(copy)
    {
        int from = lsb(copy);

        uint64_t to_bb = double_jump_sq(from);

        // Can't move onto friendly pos.pieces
        to_bb &= ~pos.pieces[pos.turn];

        // Can't move onto unfriendly pos.pieces
        to_bb &= ~pos.pieces[!pos.turn];

        // Can't move onto blockers
        to_bb &= ~pos.blockers;

        while(to_bb)
        {
            int to = lsb(to_bb);

            moves[num_moves] = (Move){.from=from, .to=to};
            num_moves++;

            to_bb &= to_bb - 1;
        }

        copy &= copy - 1;
    }

    uint64_t singles = single_jump_bb(pos.pieces[pos.turn]);

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

        moves[num_moves] = (Move){.from=to, .to=to};
        num_moves++;

        singles &= singles - 1;
    }

    assert(num_moves >= 0);
    assert(num_moves < 256);

#ifndef NDEBUG
    for(int i = 0; i < num_moves; ++i)
    {
        // Either one or the other
        assert((is_single(moves[i]) ^ is_double(moves[i])) != 0);

        // Destination needs to be empty
        assert(((pos.pieces[pos.turn] | pos.pieces[!pos.turn] | pos.blockers) & ((1ULL)<<moves[i].to)) == 0ULL);

        if(is_double(moves[i]) == true)
        {
            // Source needs to have our stone on it
            assert((pos.pieces[pos.turn] & ((1ULL)<<moves[i].from)) != 0ULL);
        }

        assert(legal_move(pos, moves[i]) == true);
    }
#endif

    return num_moves;
}
