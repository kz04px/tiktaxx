#include "zobrist.hpp"
#include <random>

uint64_t turnKey;
uint64_t pieceKeys[2][49];

uint64_t getTurnKey()
{
    return turnKey;
}

uint64_t getPieceKey(const int side, const int sq)
{
    return pieceKeys[side][sq];
}

void zobristInit()
{
    // Initialise
    std::random_device rd;
    std::mt19937_64 gen(rd());

    // Define the number generator type
    std::uniform_int_distribution<uint64_t> dis;

    turnKey = dis(gen);

    for(int n = 0; n < 2; ++n)
    {
        for(int sq = 0; sq < 49; ++sq)
        {
            pieceKeys[SIDE::CROSS][sq]  = dis(gen);
            pieceKeys[SIDE::NOUGHT][sq] = dis(gen);
        }
    }
}

uint64_t generateKey(const Position pos)
{
    uint64_t key = 0ULL;

    if(pos.turn == SIDE::CROSS)
    {
        key ^= turnKey;
    }

    for(int sq = 0; sq < 49; ++sq)
    {
        uint64_t bb = (1ULL)<<sq;

        if(bb & pos.pieces[SIDE::CROSS])
        {
            key ^= pieceKeys[SIDE::CROSS][sq];
        }
        else if(bb & pos.pieces[SIDE::NOUGHT])
        {
            key ^= pieceKeys[SIDE::NOUGHT][sq];
        }
    }

    return key;
}
