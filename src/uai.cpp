#include <ctime>
#include <iostream>
#include <libataxx/position.hpp>
#include <thread>
#include "eval.hpp"
#include "options.hpp"
#include "other.hpp"
#include "print.hpp"
#include "rollout.hpp"
#include "score.hpp"
#include "search.hpp"

void uai() {
    Options options;

    std::cout << "id name Tiktaxx" << std::endl;
    std::cout << "id author kz04px" << std::endl;

    std::cout << "option name Hash type spin default " << options.hash
              << " min 1 max 1024" << std::endl;
    std::cout << "option name Threads type spin default " << options.threads
              << " min 1 max 1" << std::endl;
    std::cout << "option name Contempt type spin default " << options.contempt
              << " min -1000000 max 1000000" << std::endl;
    // std::cout << "option name Ponder type check default " << (options.ponder
    // ? "true" : "false") << std::endl;

    std::cout << "uaiok" << std::endl;

    libataxx::Position pos{"startpos"};

    Hashtable tt;
    table_init(&tt);

    bool initialised = false;
    bool stop = false;
    std::thread search_thread;

    bool quit = false;
    while (quit == false) {
        std::string input;
        getline(std::cin, input);

        std::vector<std::string> tokens = split(input, ' ');

        for (unsigned int n = 0; n < tokens.size(); ++n) {
            if (tokens[n] == "isready") {
                if (initialised == false) {
                    table_create(&tt, options.hash);
                    initialised = true;
                }
                std::cout << "readyok" << std::endl;
            } else if (tokens[n] == "uainewgame") {
                pos.set_fen("startpos");
                table_clear(&tt);
            } else if (tokens[n] == "go") {
                // Stop the search if there's already one going
                if (search_thread.joinable()) {
                    stop = true;
                    search_thread.join();
                    stop = false;
                }

                // Default subcommands
                int depth = 0;
                int movetime = 0;
                int nodes = 0;

                // Subcommands
                for (unsigned int i = n + 1; i < tokens.size(); ++i) {
                    if (tokens[i] == "infinite") {
                        depth = 20;
                        n += 1;
                    } else if (tokens[i] == "depth") {
                        depth = stoi(tokens[i + 1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    } else if (tokens[i] == "movetime") {
                        depth = 0;
                        movetime = stoi(tokens[i + 1]);
                        n += 2;
                        i += 1;
                    } else if (tokens[i] == "nodes") {
                        nodes = stoi(tokens[i + 1]);
                        movetime = 0;
                        n += 2;
                        i += 1;
                    }
                }

                if (options.search == "mcts-uct") {
                    search_thread = std::thread(mcts_uct, pos, nodes, movetime);
                } else if (options.search == "mcts-pure") {
                    search_thread =
                        std::thread(mcts_pure, pos, nodes, movetime);
                } else if (options.search == "most-captures") {
                    // The search won't take long enough to need its own thread
                    most_captures(pos);
                } else if (options.search == "minimax") {
                    // search_thread = std::thread(minimax, &tt, &options, pos,
                    // &stop, depth, movetime);
                } else if (options.search == "random") {
                    // The search won't take long enough to need its own thread
                    random(pos);
                } else {
                    search_thread = std::thread(
                        alphabeta, &tt, &options, pos, &stop, depth, movetime);
                }
            } else if (tokens[n] == "stop") {
                // Stop the search if there's already one going
                if (search_thread.joinable()) {
                    stop = true;
                    search_thread.join();
                    stop = false;
                }
            } else if (tokens[n] == "perft") {
                int depth = 5;

                // Subcommands
                if (n + 1 < tokens.size()) {
                    depth = stoi(tokens[n + 1]);
                    n += 1;
                }

                // Subcommand checking
                if (depth < 1) {
                    depth = 1;
                }

                const auto nodes = pos.perft(depth);
                std::cout << "depth " << depth;
                std::cout << " nodes " << nodes;
                std::cout << std::endl;
            } else if (tokens[n] == "rollout") {
                if (n + 1 >= tokens.size()) {
                    continue;
                }

                // Subcommands
                int num_games = stoi(tokens[n + 1]);
                n += 1;

                // Subcommand checking
                if (num_games <= 1) {
                    num_games = 1;
                }

                float score = 0.0;

                clock_t start = clock();
                for (int g = 0; g < num_games; ++g) {
                    score += rollout(pos, 400);
                    double time_taken =
                        (double)(clock() - start) / CLOCKS_PER_SEC;

                    if ((g + 1) % 1000 == 0) {
                        std::cout << "info"
                                  << " nodes " << g + 1 << " score "
                                  << 100.0 * score / (g + 1) << "%";
                        if (time_taken > 0.0) {
                            std::cout << " nps "
                                      << (uint64_t)((g + 1) / time_taken);
                        }
                        std::cout << " time " << (uint64_t)(1000.0 * time_taken)
                                  << std::endl;
                    }
                }

                std::cout << "winrate " << 100.0 * score / num_games << "%"
                          << std::endl;
            } else if (tokens[n] == "hashtable") {
                if (n + 1 >= tokens.size()) {
                    continue;
                }

                if (tokens[n + 1] == "clear") {
                    n += 1;
                    table_clear(&tt);
                } else if (tokens[n + 1] == "print") {
                    n += 1;
                    print_details(&tt);
                }
            } else if (tokens[n] == "print") {
                print(pos, true);
            } else if (tokens[n] == "position") {
                if (n + 1 < tokens.size()) {
                    n += 1;
                    if (tokens[n] == "startpos") {
                        pos.set_fen("startpos");
                    } else if (tokens[n] == "fen") {
                        if (n + 1 < tokens.size()) {
                            n += 1;
                            std::string fen_string = tokens[n];

                            while (n + 1 < tokens.size() &&
                                   tokens[n + 1] != "moves") {
                                n += 1;
                                fen_string += " " + tokens[n];
                            }

                            pos.set_fen(fen_string);
                        }
                    }
                }
            } else if (tokens[n] == "eval") {
                split_eval(pos);
            } else if (tokens[n] == "movegen") {
                libataxx::Move moves[libataxx::max_moves];
                const auto num_moves = pos.legal_moves(moves);
                for (int i = 0; i < num_moves; ++i) {
                    std::cout << i << ": " << moves[i] << std::endl;
                }
            } else if (tokens[n] == "moves") {
                for (unsigned int i = n + 1; i < tokens.size(); ++i) {
                    try {
                        const auto move = libataxx::Move::from_uai(tokens[i]);
                        pos.makemove(move);
                        n++;
                    } catch (...) {
                        break;
                    }

                    n += 1;
                }
            } else if (tokens[n] == "options") {
                options.print();
            } else if (tokens[n] == "setoption") {
                if (n + 4 >= tokens.size() || tokens[n + 1] != "name" ||
                    tokens[n + 3] != "value") {
                    continue;
                }

                std::string option = tokens[n + 2];

                if (option == "Hash") {
                    int value = stoi(tokens[n + 4]);

                    if (value < 1) {
                        value = 1;
                    } else if (value > 1024) {
                        value = 1024;
                    }

                    options.hash = table_create(&tt, value);
                } else if (option == "Search") {
                    options.search = tokens[n + 4];
                } else if (option == "Contempt") {
                    options.contempt = stoi(tokens[n + 4]);
                } else if (option == "Threads") {
                    options.threads = stoi(tokens[n + 4]);
                } else if (option == "Ponder") {
                    options.ponder = false;
                }

                n += 4;
            } else if (tokens[n] == "quit") {
                quit = true;
                break;
            } else {
                std::cout << "Unknown token (" << tokens[n] << ")" << std::endl;
            }
        }
    }

    if (search_thread.joinable()) {
        stop = true;
        search_thread.join();
        stop = false;
    }

    table_clear(&tt);
    table_remove(&tt);
}
