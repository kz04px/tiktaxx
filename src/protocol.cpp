#include <iostream>
#include <ctime>
#include <thread>

#include "search.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "rollout.hpp"
#include "eval.hpp"
#include "makemove.hpp"
#include "score.hpp"
#include "options.hpp"
#include "other.hpp"

void messageLoop()
{
    Options options;

    std::cout << "id name Tiktaxx" << std::endl;
    std::cout << "id author kz04px" << std::endl;

    std::cout << "option name Hash type spin default " << options.hash << " min 1 max 1024" << std::endl;
    std::cout << "option name Threads type spin default " << options.threads << " min 1 max 1" << std::endl;
    std::cout << "option name Contempt type spin default " << options.contempt << " min -1000000 max 1000000" << std::endl;
    //std::cout << "option name Ponder type check default " << (options.ponder ? "true" : "false") << std::endl;

    std::cout << "uaiok" << std::endl;

    Position pos;
    setBoard(pos, "startpos");

    Hashtable tt;
    tableInit(&tt);
    tableCreate(&tt, options.hash);

    bool stop = false;
    std::thread searchThread;

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
            else if(tokens[n] == "uainewgame")
            {
                setBoard(pos, "startpos");
                tableClear(&tt);
            }
            else if(tokens[n] == "go")
            {
                // Stop the search if there's already one going
                if(searchThread.joinable())
                {
                    stop = true;
                    searchThread.join();
                    stop = false;
                }

                // Default subcommands
                int depth = 5;
                int movetime = 0;
                int numSimulations = 1000;

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
                    else if(tokens[i] == "simulations")
                    {
                        numSimulations = stoi(tokens[i+1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    }
                }

                if(options.search == "mcts-uct")
                {
                    searchThread = std::thread(mctsUCT, pos, numSimulations, movetime);
                }
                else if(options.search == "mcts-pure")
                {
                    searchThread = std::thread(mctsPure, pos, numSimulations, movetime);
                }
                else if(options.search == "most-captures")
                {
                    searchThread = std::thread(mostCaptures, pos);
                }
                else if(options.search == "minimax")
                {
                    //searchThread = std::thread(minimax, &tt, &options, pos, &stop, depth, movetime);
                }
                else if(options.search == "random")
                {
                    //searchThread = std::thread(random, pos);
                }
                else
                {
                    searchThread = std::thread(alphabeta, &tt, &options, pos, &stop, depth, movetime);
                }
            }
            else if(tokens[n] == "stop")
            {
                // Stop the search if there's already one going
                if(searchThread.joinable())
                {
                    stop = true;
                    searchThread.join();
                    stop = false;
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
                    tableClear(&tt);
                }
                else if(tokens[n+1] == "print")
                {
                    n += 1;
                    printDetails(&tt);
                }
            }
            else if(tokens[n] == "print")
            {
                print(pos, true);
            }
            else if(tokens[n] == "position")
            {
                if(n+1 < tokens.size())
                {
                    n += 1;
                    if(tokens[n] == "startpos")
                    {
                        int r = setBoard(pos, "startpos");
                        if(r != 0)
                        {
                            std::cout << "WARNING: set position error (" << r << ")" << std::endl;
                        }
                    }
                    else if(tokens[n] == "fen")
                    {
                        if(n+1 < tokens.size())
                        {
                            n += 1;
                            std::string fenString = tokens[n];

                            while(n+1 < tokens.size() && tokens[n+1] != "moves")
                            {
                                n += 1;
                                fenString += " " + tokens[n];
                            }

                            int r = setBoard(pos, fenString);
                            if(r != 0)
                            {
                                std::cout << "WARNING: set position error (" << r << ")" << std::endl;
                            }
                        }
                    }
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
            else if(tokens[n] == "options")
            {
                options.print();
            }
            else if(tokens[n] == "setoption")
            {
                if(n+4 >= tokens.size() || tokens[n+1] != "name" || tokens[n+3] != "value")
                {
                    continue;
                }

                std::string option = tokens[n+2];

                if(option == "Hash")
                {
                    int value = stoi(tokens[n+4]);

                    if(value < 1) {value = 1;}
                    else if(value > 1024) {value = 1024;}

                    options.hash = tableCreate(&tt, value);
                }
                else if(option == "Search")
                {
                    options.search = tokens[n+4];
                }
                else if(option == "Contempt")
                {
                    options.contempt = stoi(tokens[n+4]);
                }
                else if(option == "Threads")
                {
                    options.threads = stoi(tokens[n+4]);
                }
                else if(option == "Ponder")
                {
                    options.ponder = false;
                }

                n += 4;
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

    if(searchThread.joinable())
    {
        stop = true;
        searchThread.join();
        stop = false;
    }

    tableClear(&tt);
    tableRemove(&tt);
}