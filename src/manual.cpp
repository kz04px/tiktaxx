#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

#include "ataxx.hpp"
#include "hashtable.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "search.hpp"
#include "protocol.hpp"
#include "options.hpp"

std::string getEngineMove(Hashtable *tt, Options *options, Position& pos, int depth, int movetime, bool verbose=false)
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
    std::string bestMove = "";
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
            bestMove = line.substr(9);
            break;
        }
    }

    return bestMove;
}

void manual()
{
    Position pos;
    setBoard(pos, "startpos");

    Options options;

    Hashtable tt;
    tableInit(&tt);
    tableCreate(&tt, options.hash);

    // Give the user the choice of either side
    char side = ' ';
    do
    {
        std::cout << "Do you want to play X or O?" << std::endl;
        std::cin >> side;
        std::cout << std::endl;
    }
    while(side != 'X' && side != 'O' && side != 'x' && side != 'o');

    bool engineTurn = false;
    if(side == 'x' || side == 'X')
    {
        engineTurn = false;
    }
    else
    {
        engineTurn = true;
    }

    std::cout << std::endl;

    while(true)
    {
        if(engineTurn == true)
        {
            std::cout << "Thinking..." << std::endl;
            std::string moveString = getEngineMove(&tt, &options, pos, 0, 5000);
            std::cout << "Engine move: " << moveString << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;

            makemove(pos, moveString);
            engineTurn = false;
        }
        else
        {
            print(pos, false);

            std::cout << "Enter move: ";
            std::string moveString;
            std::cin >> moveString;

            if(moveString == "quit")
            {
                break;
            }
            else if(legalMove(pos, moveString) == false)
            {
                std::cout << std::endl;
                std::cout << std::endl;
                std::cout << "Illegal move" << std::endl;
            }
            else
            {
                makemove(pos, moveString);
                engineTurn = !engineTurn;
            }

            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    tableRemove(&tt);
}