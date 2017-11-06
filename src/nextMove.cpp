#include <climits>

#include "nextMove.hpp"

bool nextMove(const Move *moves, const int numMoves, Move& move, int *scores)
{
    int bestIndex = 0;
    int bestScore = INT_MIN;

    for(int n = 0; n < numMoves; ++n)
    {
        if(scores[n] > bestScore)
        {
            bestScore = scores[n];
            bestIndex = n;
        }
    }

    scores[bestIndex] = INT_MIN;
    move = moves[bestIndex];

    return (bestScore != INT_MIN);
}
