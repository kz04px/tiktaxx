#ifndef BITBOARDS_HPP_INCLUDED
#define BITBOARDS_HPP_INCLUDED

#include <cstdint>

#define U64_BOARD (0x1FFFFFFFFFFFF)

#define U64_FILE_A (0x40810204081)
#define U64_FILE_B (0x81020408102)
#define U64_FILE_C (0x102040810204)
#define U64_FILE_D (0x204081020408)
#define U64_FILE_E (0x408102040810)
#define U64_FILE_F (0x810204081020)
#define U64_FILE_G (0x1020408102040)

#define U64_NOT_FILE_A (0x1fbf7efdfbf7e)
#define U64_NOT_FILE_B (0x1f7efdfbf7efd)
#define U64_NOT_FILE_C (0x1efdfbf7efdfb)
#define U64_NOT_FILE_D (0x1dfbf7efdfbf7)
#define U64_NOT_FILE_E (0x1bf7efdfbf7ef)
#define U64_NOT_FILE_F (0x17efdfbf7efdf)
#define U64_NOT_FILE_G (0xfdfbf7efdfbf)

//const uint64_t singleJump[64] = {0ULL};
//const uint64_t doubleJump[64] = {0ULL};
const uint64_t file[7] = {U64_FILE_A, U64_FILE_B, U64_FILE_C, U64_FILE_D, U64_FILE_E, U64_FILE_F, U64_FILE_G};
//const uint64_t rank[7] = {U64_RANK_1, U64_RANK_2, U64_RANK_3, U64_RANK_4, U64_RANK_5, U64_RANK_6, U64_RANK_7};

uint64_t singleJump(uint64_t n);
uint64_t doubleJump(uint64_t n);

#endif