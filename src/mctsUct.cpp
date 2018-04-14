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
#include <cassert>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

typedef struct State
{
    Position pos;
    int numMoves;
    int currMove;
    Move moves[256];

    int moves_left()
    {
        return numMoves - currMove;
    }

    Move next()
    {
        currMove++;
        return moves[currMove-1];
    }

    void move(const Move move)
    {
        makemove(pos, move);
    }

    State()
    {
    }

    State(Position position)
    {
        pos = position;
        numMoves = movegen(position, moves);
        currMove = 0;
    }
} State;

typedef struct Node
{
    State state;
    Move move;
    int wins;
    int visits;
    Node *parent;
    std::vector<Node> children;

    Node* expand()
    {
        assert(state.moves_left() > 0);

        Move move = state.next();
        Position npos = state.pos;
        makemove(npos, move);

        State nstate = State(npos);
        Node child = Node(nstate, this, move);
        children.push_back(child);
        return &children[children.size()-1];
    }

    bool terminal()
    {
        return children.size() == 0 && state.moves_left() == 0;
    }

    Node(State s, Node *p, Move m) : state(s), move(m), wins(0), visits(1), parent(p), children({})
    {
    }
} Node;


Node* best_child(Node *node, float c)
{
    assert(node != NULL);

    int best = -10000;
    std::vector<Node> best_children;
    for(auto child : node->children)
    {
        int score = child.wins / child.visits + c * sqrt(2 * log(child.parent->visits));
        if(score > best)
        {
            best = score;
            best_children.clear();
            best_children.push_back(child);
        }
        else if(score == best)
        {
            best_children.push_back(child);
        }
    }

    assert(best_children.size() > 0);

    int index = rand() % best_children.size();
    for(unsigned int i = 0; i < node->children.size(); ++i)
    {
        if(best_children[index].move == node->children[i].move)
        {
            return &node->children[i];
        }
    }

    return NULL;
}


Node* tree_policy(Node *node)
{
    assert(node != NULL);

    while(node->terminal() == false)
    {
        if(node->state.moves_left() > 0)
        {
            return node->expand();
        }
        else
        {
            node = best_child(node, sqrt(2));
        }
    }

    return node;
}


int default_policy(Position &pos)
{
    return rollout(pos, 400);
}


void backup_negamax(Node *node, int delta)
{
    assert(node != NULL);

    while(node != NULL)
    {
        node->visits += 1;
        node->wins += delta;
        delta = -delta;
        node = node->parent;
    }
}


int getPV(Node *node, Move *moves)
{
    assert(node != NULL);
    assert(moves != NULL);

    int pvLength = 0;
    while(node->children.size())
    {
        int bestIndex = 0;
        double bestScore = 0.0;

        for(unsigned int n = 0; n < node->children.size(); ++n)
        {
            double score = node->children[n].wins / node->children[n].visits;

            if(score >= bestScore)
            {
                bestIndex = n;
                bestScore = score;
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


void printTree(Node node, int depth)
{
    if(depth > 8)
    {
        return;
    }

    for(auto child : node.children)
    {
        for(int i = 0; i < depth; ++i)
        {
            std::cout << "            ";
        }
        std::cout << moveString(child.move) << " (" << child.wins << "/" << child.visits << ")" << std::endl;

        if(child.children.size() > 0)
        {
            printTree(child, depth+1);
        }
    }
}


void mctsUCT(const Position &pos, int numSimulations, int movetime)
{
    Node root = Node(pos, NULL, NO_MOVE);
    int iteration = 0;

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

    while(iteration < numSimulations && clock() < endTarget)
    {
        Node *node = tree_policy(&root);
        int score = default_policy(node->state.pos);
        backup_negamax(node, score);
        iteration++;

        if(iteration == 1 || iteration % 1000 == 0)
        {
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;
            Move moves[64];
            int pvLength = getPV(&root, moves);
            std::string pvString = getPvString(moves, pvLength);

            std::cout << "info"
                      << " sims " << iteration
                      << " sps " << (uint64_t)(iteration/time)
                      << " time " << (uint64_t)(1000.0*time)
                      << " pv " << pvString
                      << std::endl;
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
            std::cout << "bestmove 0000" << std::endl;
        }
    }
}