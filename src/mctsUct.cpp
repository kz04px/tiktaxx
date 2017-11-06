/*
 * Monte Carlo Tree Search -- Upper Confidence Bound applied to Trees (UCT)
 * -- 
 * -- 
 * -- 
 *
 *
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

typedef struct Node
{
    Position pos;
    Move move;
    int numWins;
    int numVisits;
    Node *parent;
    std::vector<Node> children;
} Node;

typedef struct
{
    Node root;
} Tree;

double UCT(const int numChildVisits, const int numChildWins, const int numParentVisits)
{
    return ((double)numChildWins/numChildVisits) + 1.41 * sqrt(log(numParentVisits) / (double)numChildVisits);
}

int getPV(Node *node, Move *moves)
{
    int pvLength = 0;
    while(node->children.size())
    {
        int bestIndex = 0;
        double bestWinrate = 0.0;

        for(unsigned int n = 0; n < node->children.size(); ++n)
        {
            double winrate = (double)node->children[n].numWins / node->children[n].numVisits;

            if(winrate >= bestWinrate)
            {
                bestIndex = n;
                bestWinrate = winrate;
            }
        }

        if(pvLength < 256)
        {
            moves[pvLength] = node->children[bestIndex].move;
            pvLength++;
        }

        // Move on to the best child node
        node = &(node->children[bestIndex]);
    }

    return pvLength;
}

int findBestNode(Node *node)
{
    int bestIndex = -1;
    double bestScore = 0.0;

    for(unsigned int n = 0; n < node->children.size(); ++n)
    {
        double score = UCT(node->children[n].numVisits+1, node->children[n].numWins, node->numVisits);

        if(score >= bestScore)
        {
            bestIndex = n;
            bestScore = score;
        }
    }

    return bestIndex;
}

int expandNode(Node *node)
{
    Move moves[256];
    int numMoves = movegen(node->pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        Node newNode;
        newNode.pos = node->pos;
        newNode.move = moves[n];
        makemove(newNode.pos, moves[n]);
        newNode.numWins = 0;
        newNode.numVisits = 0;
        newNode.parent = node;

        node->children.push_back(newNode);
    }

    return numMoves;
}

void mctsUct(const Position& pos, int numSimulations, int movetime)
{
    Node root;
    root.pos = pos;
    root.numWins = 0;
    root.numVisits = 0;
    root.parent = NULL;

    clock_t start = clock();
    clock_t endTarget = clock();

    if(numSimulations == 0)
    {
        numSimulations = INT_MAX;
        endTarget = start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        endTarget = INT_MAX;
    }

    int sims = 0;
    while(sims < numSimulations && clock() < endTarget)
    {
        Node *selection = &root;


        // Step 1 -- Selection
        do
        {
            int n = findBestNode(selection);
            if(n == -1) {break;}
            selection = &(selection->children[n]);
        }
        while(selection->children.size() > 0);


        // Step 2 -- Expansion
        if(selection->numVisits >= 10)
        {
            expandNode(selection);
        }


        // Step 2.a -- Reselection
        int n = findBestNode(selection);
        if(n != -1)
        {
            selection = &(selection->children[n]);
        }


        // Step 3 -- Simulation
        int result = -rollout(selection->pos, 400);
        sims++;


        // Step 4 -- Backpropagation
        while(selection != NULL)
        {
            if(selection->pos.turn == root.pos.turn)
            {
                if(result == 1)
                {
                    selection->numWins++;
                }
            }
            else
            {
                if(result == -1)
                {
                    selection->numWins++;
                }
            }

            selection->numVisits++;

            selection = selection->parent;
        }


        // Details
        if(sims%10000 == 0)
        {
            Move moves[64];
            int pvLength = getPV(&root, moves);
            std::string pvString = getPvString(moves, pvLength);
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;

            for(unsigned int n = 0; n < root.children.size(); ++n)
            {
                if(root.children[n].move != moves[0])
                {
                    continue;
                }

                std::cout << "info"
                          << " sims " << sims
                          << " score winchance " << (double)root.children[n].numWins/root.children[n].numVisits
                          << " sps " << (uint64_t)(sims/time)
                          << " time " << 1000.0*time
                          << " pv " << pvString
                          << std::endl;
                break;
            }
        }
    }

    // Extract PV
    Move moves[256];
    int pvLength = getPV(&root, moves);

    if(pvLength > 0)
    {
        std::cout << "bestmove "
                  << moveString(moves[0])
                  << std::endl;
    }
    else
    {
        // Play a random move
        Move moves[256];
        int numMoves = movegen(pos, moves);

        if(numMoves > 0)
        {
            int n = rand() % numMoves;

            std::cout << "bestmove "
                      << moveString(moves[n])
                      << std::endl;
        }
        else
        {
            std::cout << "bestmove none" << std::endl;
        }
    }
}
