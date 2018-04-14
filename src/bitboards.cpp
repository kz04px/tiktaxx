#include "bitboards.hpp"

uint64_t singleJump(uint64_t n)
{
    return U64_BOARD & (((n>>1) & (U64_NOT_FILE_G)) | // Left 1
                        ((n<<1) & (U64_NOT_FILE_A)) | // Right 1
                         (n<<7) |                     // Up 1
                         (n>>7) |                     // Down 1
                        ((n<<8) & (U64_NOT_FILE_A)) | // Up 1 right 1
                        ((n<<6) & (U64_NOT_FILE_G)) | // Up 1 left 1
                        ((n>>8) & (U64_NOT_FILE_G)) | // Down 1 left 1
                        ((n>>6) & (U64_NOT_FILE_A))); // Down 1 right 1
}

uint64_t doubleJump(uint64_t n)
{
    return U64_BOARD & (((n<<12) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Up 2 left 2
                        ((n<<13) & (U64_NOT_FILE_G)) |                  // Up 2 left 1
                         (n<<14) |                                      // Up 2
                        ((n<<15) & (U64_NOT_FILE_A)) |                  // Up 2 right 1
                        ((n<<16) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Up 2 right 1

                        ((n>>2 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2
                        ((n<<5 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2 up 1
                        ((n>>9 ) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Left 2 down 1

                        ((n<<2 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2
                        ((n<<9 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2 up 1
                        ((n>>5 ) & (U64_NOT_FILE_A & U64_NOT_FILE_B)) | // Right 2 down 1

                        ((n>>16) & (U64_NOT_FILE_G & U64_NOT_FILE_F)) | // Down 2 left 2
                        ((n>>15) & (U64_NOT_FILE_G)) |                  // Down 2 left 1
                         (n>>14) |                                      // Down 2
                        ((n>>13) & (U64_NOT_FILE_A)) |                  // Down 2 right 1
                        ((n>>12) & (U64_NOT_FILE_A & U64_NOT_FILE_B))); // Down 2 right 2
}