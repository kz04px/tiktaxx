#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

#include "other.hpp"

template<typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int popcountll(const uint64_t n)
{
    return __builtin_popcountll(n);
}

int lsb(const uint64_t n)
{
    return __builtin_ctzll(n);
}

void printU64(const uint64_t n)
{
    for(int y = 6; y >= 0; --y)
    {
        for(int x = 0; x < 7; ++x)
        {
            int sq = y*7 + x;
            uint64_t bb = (1ULL) << sq;

            if(n & bb)
            {
                std::cout << "1";
            }
            else
            {
                std::cout << "0";
            }
        }
        std::cout << std::endl;
    }
}