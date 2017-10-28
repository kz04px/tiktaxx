#ifndef ATAXX_HPP_INCLUDED
#define ATAXX_HPP_INCLUDED

#include <string>
#include <cstdint>

struct SIDE { enum {CROSS, NOUGHT}; };
struct PIECE { enum {CROSS, NOUGHT, BLOCKER, EMPTY}; };
struct MOVETYPE { enum {SINGLE, DOUBLE}; };
enum SQUARE {a1, b1, c1, d1, e1, f1, g1,
             a2, b2, c2, d2, e2, f2, g2,
             a3, b3, c3, d3, e3, f3, g3,
             a4, b4, c4, d4, e4, f4, g4,
             a5, b5, c5, d5, e5, f5, g5,
             a6, b6, c6, d6, e6, f6, g6,
             a7, b7, c7, d7, e7, f7, g7};
const std::string squares[49] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1",
                                 "a2", "b2", "c2", "d2", "e2", "f2", "g2",
                                 "a3", "b3", "c3", "d3", "e3", "f3", "g3",
                                 "a4", "b4", "c4", "d4", "e4", "f4", "g4",
                                 "a5", "b5", "c5", "d5", "e5", "f5", "g5",
                                 "a6", "b6", "c6", "d6", "e6", "f6", "g6",
                                 "a7", "b7", "c7", "d7", "e7", "f7", "g7"};

typedef struct
{
    int ply;
} searchStack;

typedef struct
{
    int ply;
    int turn;
    std::uint64_t blockers;
    std::uint64_t pieces[2];
} Position;

void setBoard(Position& pos, const char *fen);
void print(const Position& pos);
int valid(const Position& pos);

#endif
