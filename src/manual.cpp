#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

#include "ataxx.hpp"
#include "hashtable.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "search.hpp"
#include "uai.hpp"
#include "options.hpp"

std::string get_engine_move(Hashtable *tt, Options *options, Position &pos, int depth, int movetime, bool verbose=false)
{
    assert(tt != NULL);

    // We need to eat the output of the search because the user doesn't want to read it
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    bool stop = false;
    alphabeta(tt, options, pos, &stop, depth, movetime);

    // Restore the normal output
    std::cout.rdbuf(old);

    // Looking for the "bestmove" line so we can play the move the search returns
    std::string best_move = "";
    while(!buffer.eof())
    {
        std::string line;
        std::getline(buffer, line, '\n');

        if(verbose == true)
        {
            std::cout << line << std::endl;
        }

        if(line.size() < 3) {continue;}

        if(line.substr(0, line.find_first_of(' ')) == "bestmove")
        {
            best_move = line.substr(9);
            break;
        }
    }

    return best_move;
}

void manual()
{
    Position pos;
    set_board(pos, "startpos");

    Options options;

    Hashtable tt;
    table_init(&tt);
    table_create(&tt, options.hash);

    // Give the user the choice of either side
    char side = ' ';
    do
    {
        std::cout << "Do you want to play X or O?" << std::endl;
        std::cin >> side;
        std::cout << std::endl;
    }
    while(side != 'X' && side != 'O' && side != 'x' && side != 'o');

    bool engine_turn = false;
    if(side == 'x' || side == 'X')
    {
        engine_turn = false;
    }
    else
    {
        engine_turn = true;
    }

    std::cout << std::endl;

    while(true)
    {
        if(engine_turn == true)
        {
            std::cout << "Thinking..." << std::endl;
            std::string move_string = get_engine_move(&tt, &options, pos, 0, 5000);
            std::cout << "Engine move: " << move_string << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;

            makemove(pos, move_string);
            engine_turn = false;
        }
        else
        {
            print(pos, false);

            std::cout << "Enter move: ";
            std::string move_string;
            std::cin >> move_string;

            if(move_string == "quit")
            {
                break;
            }
            else if(legal_move(pos, move_string) == false)
            {
                std::cout << std::endl;
                std::cout << std::endl;
                std::cout << "Illegal move" << std::endl;
            }
            else
            {
                makemove(pos, move_string);
                engine_turn = !engine_turn;
            }

            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    table_remove(&tt);
}
