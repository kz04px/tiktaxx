#include <iostream>

#include "ataxx.hpp"
#include "score.hpp"
#include "eval.hpp"

void setBoard(Position& pos, const char *fen)
{
    pos.pieces[PIECE::CROSS] = ((1ULL)<<(SQUARE::a7)) +
                               ((1ULL)<<(SQUARE::g1));

    pos.pieces[PIECE::NOUGHT] = ((1ULL)<<(SQUARE::a1)) +
                                ((1ULL)<<(SQUARE::g7));

    pos.blockers = ((1ULL)<<(SQUARE::c3)) +
                   ((1ULL)<<(SQUARE::e3)) +
                   ((1ULL)<<(SQUARE::c5)) +
                   ((1ULL)<<(SQUARE::e5));

    pos.turn = SIDE::CROSS;
    pos.ply = 0;
}

void print(const Position& pos)
{
    const char pieceChars[4] = {'X', 'O', ' ', '-'};

    for(int y = 6; y >= 0; --y)
    {
        for(int x = 0; x < 7; ++x)
        {
            int sq = 7*y + x;
            std::uint64_t bb = 1ULL<<sq;

                 if(pos.pieces[PIECE::CROSS]&bb)  {std::cout << pieceChars[PIECE::CROSS];}
            else if(pos.pieces[PIECE::NOUGHT]&bb) {std::cout << pieceChars[PIECE::NOUGHT];}
            else if(pos.blockers&bb)              {std::cout << pieceChars[PIECE::BLOCKER];}
            else                                  {std::cout << pieceChars[PIECE::EMPTY];}
        }
        std::cout << std::endl;
    }

    std::cout << "Turn: " << (pos.turn == SIDE::CROSS ? 'X' : 'O') << std::endl;
    std::cout << "Ply: " << pos.ply << std::endl;
    std::cout << "Eval: " << eval(pos) << std::endl;
    std::cout << "Score: " << score(pos) << std::endl;

    int validCode = valid(pos);
    if(validCode)
    {
        std::cout << "Valid: no (" << validCode << ")" << std::endl;
    }
    else
    {
        std::cout << "Valid: yes" << std::endl;
    }
}

int valid(const Position& pos)
{
    if(pos.pieces[SIDE::NOUGHT] & pos.pieces[SIDE::CROSS]) {return 1;}
    if(pos.pieces[SIDE::NOUGHT] & pos.blockers) {return 2;}
    if(pos.pieces[SIDE::CROSS]  & pos.blockers) {return 3;}
    return 0;
}
