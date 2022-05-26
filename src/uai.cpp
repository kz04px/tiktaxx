#include <ctime>
#include <iostream>
#include <libataxx/position.hpp>
#include <sstream>
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
    std::cout << "option name Search type combo default alphabeta options "
                 "alphabeta mcts-uct mcts-pure most-captures random"
              << std::endl;
    // std::cout << "option name Ponder type check default " << (options.ponder
    // ? "true" : "false") << std::endl;

    std::cout << "uaiok" << std::endl;

    bool stop = false;
    std::thread search_thread;
    libataxx::Position pos{"startpos"};
    Hashtable tt;
    table_init(&tt);

    // Get options and wait for isready
    bool quit = false;
    while (!quit) {
        std::string input;
        getline(std::cin, input);
        std::stringstream ss{input};
        std::string word;
        while (ss >> word) {
            if (word == "isready") {
                table_create(&tt, options.hash);
                std::cout << "readyok" << std::endl;
                quit = true;
                break;
            } else if (word == "setoption") {
                ss >> word;

                if (word != "name") {
                    continue;
                }

                std::string name;
                std::string value;

                while (ss >> word && word != "value") {
                    if (name.empty()) {
                        name = word;
                    } else {
                        name += " " + word;
                    }
                }

                while (ss >> word) {
                    if (value.empty()) {
                        value = word;
                    } else {
                        value += " " + word;
                    }
                }

                if (name == "Hash") {
                    int n = stoi(value);
                    if (n < 1) {
                        n = 1;
                    } else if (n > 1024) {
                        n = 1024;
                    }
                    options.hash = table_create(&tt, n);
                } else if (name == "Search") {
                    options.search = value;
                } else if (name == "Contempt") {
                    options.contempt = stoi(value);
                } else if (name == "Threads") {
                    options.threads = stoi(value);
                } else if (name == "Ponder") {
                    options.ponder = false;
                }
            } else if (word == "quit") {
                return;
            }
        }
    }

    quit = false;
    while (!quit) {
        std::string input;
        getline(std::cin, input);
        std::stringstream ss{input};
        std::string word;

        while (ss >> word) {
            if (word == "isready") {
                std::cout << "readyok" << std::endl;
            } else if (word == "uainewgame") {
                pos.set_fen("startpos");
                table_clear(&tt);
            } else if (word == "go") {
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
                int btime = 0;
                int wtime = 0;
                int binc = 0;
                int winc = 0;

                // Subcommands
                while (ss >> word) {
                    if (word == "infinite") {
                        depth = 20;
                    } else if (word == "depth") {
                        ss >> depth;
                    } else if (word == "movetime") {
                        ss >> movetime;
                    } else if (word == "nodes") {
                        ss >> nodes;
                    } else if (word == "btime") {
                        ss >> btime;
                    } else if (word == "wtime") {
                        ss >> wtime;
                    } else if (word == "binc") {
                        ss >> binc;
                    } else if (word == "winc") {
                        ss >> winc;
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
                        alphabeta, &tt, &options, pos, &stop, depth, movetime, btime, wtime, binc, winc);
                }
            } else if (word == "stop") {
                // Stop the search if there's already one going
                if (search_thread.joinable()) {
                    stop = true;
                    search_thread.join();
                    stop = false;
                }
            } else if (word == "perft") {
                int depth = 5;

                // Subcommands
                ss >> depth;

                // Subcommand checking
                if (depth < 1) {
                    depth = 1;
                }

                const auto nodes = pos.perft(depth);
                std::cout << "depth " << depth;
                std::cout << " nodes " << nodes;
                std::cout << std::endl;
            } else if (word == "rollout") {
                // Subcommands
                int num_games = 0;
                ss >> num_games;

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
            } else if (word == "hashtable") {
                ss >> word;
                if (word == "clear") {
                    table_clear(&tt);
                } else if (word == "print") {
                    print_details(&tt);
                }
            } else if (word == "print") {
                print(pos, true);
            } else if (word == "position") {
                ss >> word;
                if (word == "startpos") {
                    pos.set_fen("startpos");
                } else if (word == "fen") {
                    std::string fen;
                    while (ss >> word && word != "moves") {
                        if (fen.empty()) {
                            fen = word;
                        } else {
                            fen += " " + word;
                        }
                    }
                    pos.set_fen(fen);
                }
            } else if (word == "eval") {
                split_eval(pos);
            } else if (word == "movegen") {
                libataxx::Move moves[libataxx::max_moves];
                const auto num_moves = pos.legal_moves(moves);
                for (int i = 0; i < num_moves; ++i) {
                    std::cout << i << ": " << moves[i] << std::endl;
                }
            } else if (word == "moves") {
                while (ss >> word) {
                    try {
                        const auto move = libataxx::Move::from_uai(word);
                        pos.makemove(move);
                    } catch (...) {
                        break;
                    }
                }
            } else if (word == "options") {
                options.print();
            } else if (word == "quit") {
                quit = true;
                break;
            } else {
                std::cout << "Unknown token (" << word << ")" << std::endl;
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
