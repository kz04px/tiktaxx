#include "sorting.hpp"

#define INF (999999)

void sortMoves(const Position& pos, Move *moves, const int numMoves, const Move& ttMove)
{
    int scores[256] = {0};

    // Score
    for(int n = 0; n < numMoves; ++n)
    {
        if(moves[n] == ttMove)
        {
            scores[n] = 1000;
        }
        else
        {
            scores[n] = countCaptures(pos, moves[n]);
        }

        scores[n] += (isSingle(moves[n]) ? 1 : 0);
    }

    // Sort
    for(int a = 0; a < numMoves; ++a)
    {
        int bestIndex = a;
        int bestScore = -INF;

        for(int b = a; b < numMoves; ++b)
        {
            if(scores[b] > bestScore)
            {
                bestScore = scores[b];
                bestIndex = b;
            }
        }

        Move tempMove = moves[bestIndex];
        moves[bestIndex] = moves[a];
        moves[a] = tempMove;

        int tempScore = scores[bestIndex];
        scores[bestIndex] = scores[a];
        scores[a] = tempScore;
    }
}
