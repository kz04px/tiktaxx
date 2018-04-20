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

struct State
{
    Position pos;
    int numMoves;
    int currMove;
    Move moves[256];

    int moves_left() const
    {
        return numMoves - currMove;
    }

    Move next()
    {
        currMove++;
        return moves[currMove-1];
    }

    void move(const Move &move)
    {
        makemove(pos, move);
    }

    State() : pos(), numMoves(0), currMove(0)
    {
    }

    State(Position position) : pos(position)
    {
        numMoves = movegen(position, moves);
        currMove = 0;
    }
};

struct Node
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

    bool terminal() const
    {
        return children.size() == 0 && state.moves_left() == 0;
    }

    Node(State s, Node *p, Move m) : state(s), move(m), wins(0), visits(1), parent(p), children({})
    {
    }
};


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


int get_pv(Node *node, Move *moves)
{
    assert(node != NULL);
    assert(moves != NULL);

    int pv_length = 0;
    while(node->children.size())
    {
        int best_index = 0;
        double best_score = 0.0;

        for(unsigned int n = 0; n < node->children.size(); ++n)
        {
            double score = node->children[n].wins / node->children[n].visits;

            if(score >= best_score)
            {
                best_index = n;
                best_score = score;
            }
        }

        if(pv_length < 256)
        {
            moves[pv_length] = node->children[best_index].move;
            pv_length++;
        }

        // Move on to the best child node
        node = &(node->children[best_index]);
    }

    return pv_length;
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
        std::cout << move_string(child.move) << " (" << child.wins << "/" << child.visits << ")" << std::endl;

        if(child.children.size() > 0)
        {
            printTree(child, depth+1);
        }
    }
}


void mcts_uct(const Position &pos, int num_simulations, int movetime)
{
    Node root = Node(pos, NULL, NO_MOVE);
    int iteration = 0;

    clock_t start = clock();
    clock_t end_target = clock();

    if(num_simulations == 0)
    {
        num_simulations = INT_MAX;
        end_target = start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        end_target = INT_MAX;
    }

    while(iteration < num_simulations && clock() < end_target)
    {
        Node *node = tree_policy(&root);
        int score = default_policy(node->state.pos);
        backup_negamax(node, score);
        iteration++;

        if(iteration == 1 || iteration % 1000 == 0)
        {
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;
            Move moves[64];
            int pv_length = get_pv(&root, moves);
            std::string pv_string = get_pv_string(moves, pv_length);

            std::cout << "info"
                      << " sims " << iteration
                      << " sps " << (uint64_t)(iteration/time)
                      << " time " << (uint64_t)(1000.0*time)
                      << " pv " << pv_string
                      << std::endl;
        }
    }

    // Extract PV
    Move moves[256];
    int pv_length = get_pv(&root, moves);

    if(pv_length > 0)
    {
        std::cout << "bestmove "
                  << move_string(moves[0])
                  << std::endl;
    }
    else
    {
        // Play a random move
        Move moves[256];
        int num_moves = movegen(pos, moves);

        if(num_moves > 0)
        {
            int n = rand() % num_moves;

            std::cout << "bestmove "
                      << move_string(moves[n])
                      << std::endl;
        }
        else
        {
            std::cout << "bestmove 0000" << std::endl;
        }
    }
}
