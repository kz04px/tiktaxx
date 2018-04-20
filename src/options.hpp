#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED

#include <iostream>

struct Options
{
    Options() : contempt(0), threads(1), hash(128), ponder(false), search("alphabeta")
    {
    }

    void print()
    {
        std::cout << "Contempt: " << contempt << std::endl;
        std::cout << "Threads: " << threads << std::endl;
        std::cout << "Hash: " << hash << std::endl;
        std::cout << "Ponder: " << ponder << std::endl;
        std::cout << "Search: " << search << std::endl;
    }

    int contempt;
    int threads;
    int hash;
    bool ponder;
    std::string search;
};

#endif