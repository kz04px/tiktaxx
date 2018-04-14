#ifndef OTHER_HPP_INCLUDED
#define OTHER_HPP_INCLUDED

#include <cstdint>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);
int popcountll(const uint64_t n);
int lsb(const uint64_t n);
void printU64(const uint64_t n);

#endif