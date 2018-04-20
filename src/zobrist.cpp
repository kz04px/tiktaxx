#include <random>

#include "zobrist.hpp"
#include "other.hpp"

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
    auto seed = 1927725765;
    std::mt19937_64 gen(seed);

    // Define the number generator type
    std::uniform_int_distribution<uint64_t> dis;

    turnKey = dis(gen);

    for(int sq = 0; sq < 49; ++sq)
    {
        pieceKeys[SIDE::CROSS][sq]  = dis(gen);
        pieceKeys[SIDE::NOUGHT][sq] = dis(gen);
    }
}

uint64_t generateKey(const Position& pos)
{
    uint64_t key = 0ULL;

    if(pos.turn == SIDE::CROSS)
    {
        key ^= turnKey;
    }

    uint64_t copy = pos.pieces[SIDE::CROSS];
    while(copy)
    {
        int sq = lsb(copy);
        key ^= pieceKeys[SIDE::CROSS][sq];
        copy &= copy - 1;
    }

    copy = pos.pieces[SIDE::NOUGHT];
    while(copy)
    {
        int sq = lsb(copy);
        key ^= pieceKeys[SIDE::NOUGHT][sq];
        copy &= copy - 1;
    }

    return key;
}