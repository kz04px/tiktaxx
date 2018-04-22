#include <iostream>
#include <vector>

#include "ataxx.hpp"
#include "score.hpp"
#include "eval.hpp"
#include "phase.hpp"
#include "other.hpp"
#include "invalid.hpp"

#define STARTPOS "x5o/7/2-1-2/7/2-1-2/7/o5x x"

int set_board(Position &pos, std::string fen)
{
    if(fen == "startpos") {fen = STARTPOS;}

    std::vector<std::string> tokens = split(fen, ' ');

         if(tokens.size() == 0) {return 1;}
    else if(tokens.size() > 2)  {return 2;}

         if(tokens[0].length() < 13) {return 3;}
    else if(tokens[0].length() > 55) {return 4;}

    // Clear
    pos.pieces[PIECE::CROSS] = 0ULL;
    pos.pieces[PIECE::NOUGHT] = 0ULL;
    pos.blockers = 0ULL;
    pos.turn = SIDE::CROSS;
    pos.ply = 0;

    int sq = SQUARE::a7;

    for(char &c : tokens[0])
    {
        switch(c)
        {
            case 'x':
            case 'X':
                pos.pieces[PIECE::CROSS] ^= (1ULL << sq);
                sq++;
                break;
            case 'o':
            case 'O':
                pos.pieces[PIECE::NOUGHT] ^= (1ULL << sq);
                sq++;
                break;
            case '-':
                pos.blockers ^= (1ULL << sq);
                sq++;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                sq += c - '0';
                break;
            case '/':
                sq -= 14;
                break;
            default:
                return 5;
                break;
        }
    }

    if(tokens.size() > 1)
    {
        if(tokens[1] == "x" || tokens[1] == "X")
        {
            pos.turn = SIDE::CROSS;
        }
        else if(tokens[1] == "o" || tokens[1] == "O")
        {
            pos.turn = SIDE::NOUGHT;
        }
        else
        {
            return 6;
        }
    }

    if(sq != SQUARE::a2) {return 7;}

    if(invalid(pos) == true) {return 8;}

    return 0;
}

void print(const Position &pos, bool details)
{
    std::cout << "  a b c d e f g" << std::endl;
    std::cout << " ╔═╦═╦═╦═╦═╦═╦═╗" << std::endl;
    for(int y = 6; y >= 0; --y)
    {
        std::cout << y + 1 << "║";
        for(int x = 0; x < 7; ++x)
        {
            int sq = 7*y + x;
            std::uint64_t bb = 1ULL<<sq;

                 if(pos.pieces[PIECE::CROSS]&bb)  {std::cout << "X";}
            else if(pos.pieces[PIECE::NOUGHT]&bb) {std::cout << "O";}
            else if(pos.blockers&bb)              {std::cout << "#";}
            else                                  {std::cout << " ";}
            std::cout << "║";
        }
        std::cout << y + 1 << std::endl;
        if(y > 0) {std::cout << " ╠═╬═╬═╬═╬═╬═╬═╣" << std::endl;}
    }
    std::cout << " ╚═╩═╩═╩═╩═╩═╩═╝" << std::endl;
    std::cout << "  a b c d e f g" << std::endl;

    if(details == true)
    {
        std::cout << "Turn: " << (pos.turn == SIDE::CROSS ? 'X' : 'O') << std::endl;
        std::cout << "Ply: " << pos.ply << std::endl;
        std::cout << "Eval: " << eval(pos) << std::endl;
        std::cout << "Score: " << score(pos) << std::endl;
        std::cout << "Phase: " << phase(pos) << std::endl;
        std::cout << "Endgame: " << (is_endgame(pos) == true ? "true" : "false") << std::endl;

        int r = invalid(pos);
        if(r == true)
        {
            std::cout << "Valid: false (" << r << ")" << std::endl;
        }
        else
        {
            std::cout << "Valid: true" << std::endl;
        }
    }
}
