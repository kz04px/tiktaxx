#include "bitboards.hpp"

uint64_t single_jump_bb(const uint64_t bb)
{
    return U64_BOARD & (((bb>>1) & (U64_NOT_FILE_G)) | // Left 1
                        ((bb<<1) & (U64_NOT_FILE_A)) | // Right 1
                         (bb<<7) |                     // Up 1
                         (bb>>7) |                     // Down 1
                        ((bb<<8) & (U64_NOT_FILE_A)) | // Up 1 right 1
                        ((bb<<6) & (U64_NOT_FILE_G)) | // Up 1 left 1
                        ((bb>>8) & (U64_NOT_FILE_G)) | // Down 1 left 1
                        ((bb>>6) & (U64_NOT_FILE_A))); // Down 1 right 1
}

uint64_t double_jump_bb(const uint64_t bb)
{
    return U64_BOARD & (((bb<<12) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Up 2 left 2
                        ((bb<<13) & (U64_NOT_FILE_G)) |                  // Up 2 left 1
                         (bb<<14) |                                      // Up 2
                        ((bb<<15) & (U64_NOT_FILE_A)) |                  // Up 2 right 1
                        ((bb<<16) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Up 2 right 1

                        ((bb>>2 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2
                        ((bb<<5 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2 up 1
                        ((bb>>9 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2 down 1

                        ((bb<<2 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2
                        ((bb<<9 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2 up 1
                        ((bb>>5 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2 down 1

                        ((bb>>16) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Down 2 left 2
                        ((bb>>15) & (U64_NOT_FILE_G)) |                  // Down 2 left 1
                         (bb>>14) |                                      // Down 2
                        ((bb>>13) & (U64_NOT_FILE_A)) |                  // Down 2 right 1
                        ((bb>>12) & (U64_NOT_FILE_A & U64_NOT_FILE_B))); // Down 2 right 2
}
