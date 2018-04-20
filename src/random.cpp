#include <cassert>

#include "search.hpp"
#include "ataxx.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"
#include "score.hpp"
#include "searchstack.hpp"

void random(const Position& pos)
{
    Move move = NO_MOVE;
    int score = 0;
    Move moves[256];
    int numMoves = movegen(pos, moves);

    if(numMoves > 0)
    {
        Position newPos = pos;

        int n = rand()%numMoves;
        move = moves[n];

        makemove(newPos, move);
        score = -eval(newPos);
    }

    std::cout << "info"
              << " nodes " << 1
              << " score " << score
              << std::endl;

    assert(legalMove(pos, move) == true);
    std::cout << "bestmove " << moveString(move) << std::endl;
}