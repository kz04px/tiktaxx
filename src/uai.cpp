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

void uai()
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
    set_board(pos, "startpos");

    Hashtable tt;
    table_init(&tt);

    bool initialised = false;
    bool stop = false;
    std::thread search_thread;

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
                if(initialised == false)
                {
                    table_create(&tt, options.hash);
                    initialised = true;
                }
                std::cout << "readyok" << std::endl;
            }
            else if(tokens[n] == "uainewgame")
            {
                set_board(pos, "startpos");
                table_clear(&tt);
            }
            else if(tokens[n] == "go")
            {
                // Stop the search if there's already one going
                if(search_thread.joinable())
                {
                    stop = true;
                    search_thread.join();
                    stop = false;
                }

                // Default subcommands
                int depth = 5;
                int movetime = 0;
                int nodes = 1000;

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
                    else if(tokens[i] == "nodes")
                    {
                        nodes = stoi(tokens[i+1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    }
                }

                if(options.search == "mcts-uct")
                {
                    search_thread = std::thread(mcts_uct, pos, nodes, movetime);
                }
                else if(options.search == "mcts-pure")
                {
                    search_thread = std::thread(mcts_pure, pos, nodes, movetime);
                }
                else if(options.search == "most-captures")
                {
                    search_thread = std::thread(most_captures, pos);
                }
                else if(options.search == "minimax")
                {
                    //search_thread = std::thread(minimax, &tt, &options, pos, &stop, depth, movetime);
                }
                else if(options.search == "random")
                {
                    // The search won't take long enough to need its own thread
                    random(pos);
                }
                else
                {
                    search_thread = std::thread(alphabeta, &tt, &options, pos, &stop, depth, movetime);
                }
            }
            else if(tokens[n] == "stop")
            {
                // Stop the search if there's already one going
                if(search_thread.joinable())
                {
                    stop = true;
                    search_thread.join();
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
                int num_moves = movegen(pos, moves);

                if(num_moves > 0)
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

                split_perft(&tt, pos, depth);
            }
            else if(tokens[n] == "rollout")
            {
                if(n+1 >= tokens.size()) {continue;}

                // Subcommands
                int num_games = stoi(tokens[n+1]);
                n += 1;

                // Subcommand checking
                if(num_games <= 1)
                {
                    num_games = 1;
                }

                int wins = 0;
                int losses = 0;
                int draws = 0;

                clock_t start = clock();
                for(int g = 0; g < num_games; ++g)
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
                    table_clear(&tt);
                }
                else if(tokens[n+1] == "print")
                {
                    n += 1;
                    print_details(&tt);
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
                        int r = set_board(pos, "startpos");
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
                            std::string fen_string = tokens[n];

                            while(n+1 < tokens.size() && tokens[n+1] != "moves")
                            {
                                n += 1;
                                fen_string += " " + tokens[n];
                            }

                            int r = set_board(pos, fen_string);
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
                split_eval(pos);
            }
            else if(tokens[n] == "movegen")
            {
                print_moves(pos);
            }
            else if(tokens[n] == "moves")
            {
                for(unsigned int i = n+1; i < tokens.size(); ++i)
                {
                    if(legal_move(pos, tokens[i]) == false) {break;}

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

                    options.hash = table_create(&tt, value);
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

    if(search_thread.joinable())
    {
        stop = true;
        search_thread.join();
        stop = false;
    }

    table_clear(&tt);
    table_remove(&tt);
}
