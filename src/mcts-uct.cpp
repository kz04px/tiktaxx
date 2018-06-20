/*
 * Monte Carlo Tree Search -- Upper Confidence Bound applied to Trees (UCT)
 * -- Heavy rollouts
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
#include <limits>
#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

struct State
{
    Position pos;
    int num_moves;
    int curr_move;
    Move moves[256];

    int moves_left() const
    {
        assert(num_moves - curr_move >= 0);

        return num_moves - curr_move;
    }

    Move next()
    {
        assert(curr_move < num_moves);

        curr_move++;
      
        assert(curr_move-1 >= 0);
        assert(curr_move-1 < num_moves);

        return moves[curr_move-1];
    }

    void move(const Move &move)
    {
        makemove(pos, move);
    }

    State() : pos(), num_moves(0), curr_move(0)
    {
    }

    State(Position position) : pos(position)
    {
        num_moves = movegen(position, moves);
        curr_move = 0;
    }
};

struct Node
{
    State state;
    Move move;
    float score;
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

    Node(State s, Node *p, Move m) : state(s), move(m), score(0.0), visits(0), parent(p), children({})
    {
    }
};

Node* best_child(Node *node, const float c)
{
    assert(node != NULL);
    assert(node->visits > 0);
    assert(node->children.size() > 0);
    assert(node->state.moves_left() == 0);

    float best = std::numeric_limits<float>::lowest();
    std::vector<Node*> best_children;

    for(Node &child : node->children)
    {
        assert(child.visits > 0);
        assert(child.score >= 0);
        assert(child.score <= child.visits);

        float exploitation = child.score/child.visits;
        float expansion = c * sqrt(2.0 * log(node->visits) / child.visits);
        float captures = (float)(count_captures(node->state.pos, child.move) + (is_single(child.move) ? 1 : 0)) / 10.0;
        float score = exploitation + expansion + captures;

        if(score > best)
        {
            best = score;
            best_children.clear();
            best_children.push_back(&child);
        }
        else if(score == best)
        {
            best_children.push_back(&child);
        }
    }

    assert(best != std::numeric_limits<float>::lowest());
    assert(best_children.size() > 0);
    assert(best_children.size() <= node->children.size());

    int index = rand() % best_children.size();

    return best_children[index];
}

Node* tree_policy(Node *node)
{
    assert(node != NULL);

    while(node->terminal() == false)
    {
        if(node->state.moves_left() > 0)
        {
            node = node->expand();
            break;
        }
        else
        {
            node = best_child(node, 0.1);
        }
    }

    assert(node != NULL);

    return node;
}

float default_policy(Position &pos)
{
    float r = 0.0;
    for(int i = 0; i < 5; ++i)
    {
        r += 1.0 - rollout_heavy(pos, 400);
    }
    return r/5.0;
}

void backup_negamax(Node *node, float delta)
{
    assert(node != NULL);
    assert(0.0 <= delta);
    assert(delta <= 1.0);

    while(node != NULL)
    {
        node->visits += 1;
        node->score += delta;
        delta = 1.0 - delta;
        node = node->parent;
    }
}

PV get_pv(Node *node)
{
    assert(node != NULL);

    PV pv;
    while(node->children.size() > 0)
    {
        int best_index = 0;
        float best_score = std::numeric_limits<float>::lowest();

        for(unsigned int n = 0; n < node->children.size(); ++n)
        {
            assert(node->children[n].visits > 0);

            float score = node->children[n].visits;

            if(score >= best_score)
            {
                best_index = n;
                best_score = score;
            }
        }

        if(pv.num_moves < 256)
        {
            pv.moves[pv.num_moves] = node->children[best_index].move;
            pv.num_moves++;
        }

        // Move on to the best child node
        node = &(node->children[best_index]);
    }

    return pv;
}

void print_tree(Node node, int depth)
{
    if(depth > 8)
    {
        return;
    }

    for(auto &child : node.children)
    {
        float score = child.score/child.visits + (0.0) * sqrt(2.0 * log(node.visits) / child.visits);
        for(int i = 0; i < depth; ++i)
        {
            std::cout << "            ";
        }
        std::cout << move_string(child.move) << " (" << child.score << "/" << child.visits << " " << score << ")" << std::endl;

        if(child.children.size() > 0)
        {
            print_tree(child, depth+1);
        }
    }
}

void mcts_uct(const Position &pos, int nodes, int movetime)
{
    Node root = Node(pos, NULL, NO_MOVE);
    int iteration = 0;

    clock_t start = clock();
    clock_t end_target = clock();

    if(nodes == 0)
    {
        nodes = std::numeric_limits<int>::max();
        end_target = start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        end_target = std::numeric_limits<int>::max();
    }

    while(iteration < nodes && clock() < end_target)
    {
        Node *node = tree_policy(&root);
        float score = default_policy(node->state.pos);
        backup_negamax(node, score);
        iteration++;

        if(iteration == 1 || iteration % 1000 == 0)
        {
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;
            PV pv = get_pv(&root);
            assert(legal_pv(pos, pv) == true);
            std::string pv_string = get_pv_string(pv);

            std::cout << "info"
                      << " nodes " << iteration;
            if(time > 0.0)
            {
                std::cout << " nps " << (uint64_t)(iteration/time);
            }
            std::cout << " time " << (uint64_t)(1000.0*time)
                      << " pv " << pv_string
                      << std::endl;
        }
    }

    // Extract PV
    PV pv = get_pv(&root);

    if(pv.num_moves > 0)
    {
        std::cout << "bestmove "
                  << move_string(pv.moves[0])
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
