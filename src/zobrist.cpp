#include <random>

#include "zobrist.hpp"
#include "other.hpp"

uint64_t turn_key;
uint64_t piece_keys[2][49];

uint64_t get_turn_key()
{
    return turn_key;
}

uint64_t get_piece_key(const int side, const int sq)
{
    return piece_keys[side][sq];
}

void zobrist_init()
{
    // Initialise
    auto seed = 1927725765;
    std::mt19937_64 gen(seed);

    // Define the number generator type
    std::uniform_int_distribution<uint64_t> dis;

    turn_key = dis(gen);

    for(int sq = 0; sq < 49; ++sq)
    {
        piece_keys[SIDE::CROSS][sq]  = dis(gen);
        piece_keys[SIDE::NOUGHT][sq] = dis(gen);
    }
}

uint64_t generate_key(const Position &pos)
{
    uint64_t key = 0ULL;

    if(pos.turn == SIDE::CROSS)
    {
        key ^= turn_key;
    }

    uint64_t copy = pos.pieces[SIDE::CROSS];
    while(copy)
    {
        int sq = lsb(copy);
        key ^= piece_keys[SIDE::CROSS][sq];
        copy &= copy - 1;
    }

    copy = pos.pieces[SIDE::NOUGHT];
    while(copy)
    {
        int sq = lsb(copy);
        key ^= piece_keys[SIDE::NOUGHT][sq];
        copy &= copy - 1;
    }

    return key;
}
