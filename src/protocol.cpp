#include <iostream>
#include <ctime>

#include "search.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "rollout.hpp"
#include "eval.hpp"
#include "makemove.hpp"
#include "score.hpp"
#include "other.hpp"

void messageLoop()
{
    Position pos;
    setBoard(pos, "startpos");

    Hashtable tt;
    tableInit(&tt);
    create(&tt, 128);

    bool quit = false;
    while(quit == false)
    {
        std::string input;
        getline(std::cin, input);

        std::vector<std::string> tokens = split(input, ' ');

        for(unsigned int n = 0; n < tokens.size(); ++n)
        {
            if(tokens[n] == "isready")
            {
                std::cout << "readyok" << std::endl;
            }
            else if(tokens[n] == "go")
            {
                // Default subcommands
                int depth = 5;
                int movetime = 0;

                // Subcommands
                for(unsigned int i = n+1; i < tokens.size(); ++i)
                {
                    if(tokens[i] == "infinite")
                    {
                        depth = 20;
                        n += 1;
                    }
                    else if(tokens[i] == "depth")
                    {
                        depth = stoi(tokens[i+1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    }
                    else if(tokens[i] == "movetime")
                    {
                        depth = 0;
                        movetime = stoi(tokens[i+1]);
                        n += 2;
                        i += 1;
                    }
                }

                search(&tt, pos, depth, movetime);
            }
            else if(tokens[n] == "mcts")
            {
                // Default subcommands
                int numSimulations = 100000;
                int movetime = 0;
                int searchType = 1;

                // Subcommands
                for(unsigned int i = n+1; i < tokens.size(); ++i)
                {
                    if(tokens[i] == "pure")
                    {
                        searchType = 0;
                        n += 1;
                    }
                    else if(tokens[i] == "uct")
                    {
                        searchType = 1;
                        n += 1;
                    }
                    else if(tokens[i] == "movetime")
                    {
                        numSimulations = 0;
                        movetime = stoi(tokens[i+1]);
                        n += 2;
                        i += 1;
                    }
                    else if(tokens[i] == "simulations")
                    {
                        numSimulations = stoi(tokens[i+1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    }
                }

                if(searchType == 0)
                {
                    mctsPure(pos, numSimulations, movetime);
                }
                else if(searchType == 1)
                {
                    mctsUct(pos, numSimulations, movetime);
                }
            }
            else if(tokens[n] == "perft")
            {
                int depth = 5;

                // Subcommands
                if(n+1 < tokens.size())
                {
                    depth = stoi(tokens[n+1]);
                    n += 1;
                }

                // Subcommand checking
                if(depth < 1)
                {
                    depth = 1;
                }
                else if(depth > 12)
                {
                    std::cout << "WARNING: perft(" << depth << ") may take a long time to finish" << std::endl;
                }

                perft(&tt, pos, depth);
            }
            else if(tokens[n] == "result")
            {
                Move moves[256];
                int numMoves = movegen(pos, moves);

                if(numMoves > 0)
                {
                    std::cout << "result none" << std::endl;
                }
                else
                {
                    int r = score(pos);

                    if(r == 0) {std::cout << "result draw" << std::endl; continue;}

                    if(pos.turn == SIDE::CROSS)
                    {
                        if(r > 0) {std::cout << "result X" << std::endl;}
                        else      {std::cout << "result O" << std::endl;}
                    }
                    else
                    {
                        if(r > 0) {std::cout << "result O" << std::endl;}
                        else      {std::cout << "result X" << std::endl;}
                    }
                }
            }
            else if(tokens[n] == "split")
            {
                int depth = 5;

                // Subcommands
                if(n+1 < tokens.size())
                {
                    depth = stoi(tokens[n+1]);
                    n += 1;
                }

                // Subcommand checking
                if(depth < 1)
                {
                    depth = 1;
                }
                else if(depth > 12)
                {
                    std::cout << "WARNING: split perft(" << depth << ") may take a long time to finish" << std::endl;
                }

                splitPerft(&tt, pos, depth);
            }
            else if(tokens[n] == "rollout")
            {
                if(n+1 >= tokens.size()) {continue;}

                // Subcommands
                int numGames = stoi(tokens[n+1]);
                n += 1;

                // Subcommand checking
                if(numGames <= 1)
                {
                    numGames = 1;
                }

                int wins = 0;
                int losses = 0;
                int draws = 0;

                clock_t start = clock();
                for(int g = 0; g < numGames; ++g)
                {
                    int r = rollout(pos, 300);
                         if(r == 1)  {wins++;}
                    else if(r == -1) {losses++;}
                    else             {draws++;}

                    if((g+1) % 100 == 0)
                    {
                        std::cout << "info"
                                  << " wins " << wins
                                  << " draws " << draws
                                  << " losses " << losses
                                  << " winrate " << 100.0*(double)wins/(wins + draws + losses) << "%"
                                  << " time " << 1000.0*(double)(clock() - start)/CLOCKS_PER_SEC
                                  << std::endl;
                    }
                }

                std::cout << "winrate "
                          << 100.0*(double)wins/(wins + draws + losses) << "%"
                          << std::endl;
            }
            else if(tokens[n] == "hashtable")
            {
                if(n+1 >= tokens.size()) {continue;}

                if(tokens[n+1] == "clear")
                {
                    n += 1;
                    clear(&tt);
                }
                else if(tokens[n+1] == "print")
                {
                    n += 1;
                    printDetails(&tt);
                }
            }
            else if(tokens[n] == "print")
            {
                print(pos);
            }
            else if(tokens[n] == "setpos")
            {
                if(n+2 < tokens.size())
                {
                    int r = setBoard(pos, tokens[n+1] + " " + tokens[n+2]);
                    if(r != 0)
                    {
                        std::cout << "WARNING: set position error (" << r << ")" << std::endl;
                    }

                    n += 2;
                }
                else if(n+1 < tokens.size())
                {
                    if(tokens[n+1] != "startpos") {tokens[n+1] += " X";}

                    int r = setBoard(pos, tokens[n+1]);
                    if(r != 0)
                    {
                        std::cout << "WARNING: set position error (" << r << ")" << std::endl;
                    }

                    n += 1;
                }
            }
            else if(tokens[n] == "eval")
            {
                splitEval(pos);
            }
            else if(tokens[n] == "movegen")
            {
                printMoves(pos);
            }
            else if(tokens[n] == "moves")
            {
                for(unsigned int i = n+1; i < tokens.size(); ++i)
                {
                    if(legalMove(pos, tokens[i]) == false) {break;}

                    makemove(pos, tokens[i]);
                    n += 1;
                }
            }
            else if(tokens[n] == "help")
            {
                std::cout << "Commands:" << std::endl;
                std::cout << "-- go [depth] [movetime]" << std::endl;
                std::cout << "-- perft [depth]" << std::endl;
                std::cout << "-- split [depth]" << std::endl;
                std::cout << "-- rollout [games]" << std::endl;
                std::cout << "-- hashtable [clear/print]" << std::endl;
                std::cout << "-- print" << std::endl;
                std::cout << "-- setpos [position]" << std::endl;
                std::cout << "-- eval" << std::endl;
                std::cout << "-- movegen" << std::endl;
                std::cout << "-- moves [moves]" << std::endl;
                std::cout << "-- isready" << std::endl;
                std::cout << "-- help" << std::endl;
                std::cout << "-- about" << std::endl;
                std::cout << "-- quit" << std::endl;
            }
            else if(tokens[n] == "about")
            {
                std::cout << "An Ataxx engine written in C++" << std::endl;
                std::cout << "https://github.com/kz04px/ataxx-engine" << std::endl;
            }
            else if(tokens[n] == "quit")
            {
                quit = true;
                break;
            }
            else
            {
                std::cout << "Unknown token (" << tokens[n] << ")" << std::endl;
            }
        }
    }

    clear(&tt);
    tableRemove(&tt);
}
