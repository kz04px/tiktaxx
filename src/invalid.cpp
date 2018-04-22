#include "invalid.hpp"
#include "bitboards.hpp"

int invalid(const Position &pos)
{
    if(pos.pieces[SIDE::NOUGHT] & pos.pieces[SIDE::CROSS]) {return 1;}
    if(pos.pieces[SIDE::NOUGHT] & pos.blockers) {return 2;}
    if(pos.pieces[SIDE::CROSS] & pos.blockers) {return 3;}
    if(pos.pieces[SIDE::NOUGHT] & ~U64_BOARD) {return 4;}
    if(pos.pieces[SIDE::CROSS] & ~U64_BOARD) {return 5;}
    if(pos.blockers & ~U64_BOARD) {return 6;}
    return 0;
}
