#include <iostream>
#include <cstring>
#include <cassert>

#include "ataxx.hpp"
#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "hashtable.hpp"
#include "pv.hpp"
#include "searchinfo.hpp"
#include "eval.hpp"
#include "score.hpp"
#include "zobrist.hpp"
#include "sorting.hpp"
#include "next-move.hpp"
#include "searchstack.hpp"

int reduction(const int move_num, const int depth)
{
    assert(move_num >= 0);
    assert(depth >= 0);

    if(move_num < 2 || depth < 3)
    {
        return 0;
    }

    if(move_num < 6)
    {
        return 1;
    }
    else if(move_num < 12)
    {
        return depth / 3;
    }
    else
    {
        return depth / 2;
    }
}

int alphabeta_search(const Position &pos, search_info &info, search_stack *ss, PV &pv, int alpha, int beta, int depth)
{
    assert(ss != NULL);
    assert(depth >= 0);
    assert(beta >= alpha);

    if(depth == 0 || info.depth >= MAX_DEPTH)
    {
        info.leaf_nodes++;
        return eval(pos);
    }

    if(info.nodes != 0)
    {
        // Stop searching if we've ran out of time
        if(*info.stop == true || clock() >= info.end)
        {
            return 0;
        }

        // Send an update on what we're doing
        if(info.nodes % 2000000 == 0)
        {
            double time_spent = (double)(clock() - info.start)/CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes/time_spent)
                      << std::endl;
        }
    }

    int alpha_original = alpha;
    uint64_t key = generate_key(pos);
    Move tt_move = NO_MOVE;
    bool pvnode = (beta - alpha == 1);

    // Check the hash table
    Entry entry = probe(info.tt, key);
    if(key == entry.key)
    {
        tt_move = get_move(entry);

#ifndef NDEBUG
        info.hash_hits++;
        if(legal_move(pos, tt_move) == false)
        {
            info.hash_collisions++;
        }
#endif

        if(get_depth(entry) >= depth && legal_move(pos, tt_move) == true)
        {
            switch(get_flag(entry))
            {
                case EXACT:
                    pv.num_moves = 1;
                    pv.moves[0] = tt_move;
                    return get_eval(entry);
                    break;
                case LOWERBOUND:
                    alpha = (alpha > get_eval(entry) ? alpha : get_eval(entry));
                    break;
                case UPPERBOUND:
                    beta = (beta < get_eval(entry) ? beta : get_eval(entry));
                    break;
                default:
                    assert(false);
                    break;
            }

            if(alpha >= beta)
            {
                pv.num_moves = 1;
                pv.moves[0] = tt_move;
                return get_eval(entry);
            }
        }
    }

#ifdef NULLMOVE
    #define R (2)

    if(ss->nullmove && depth > 2 && !pvnode)
    {
        PV new_pv;

        Position new_pos = pos;
        new_pos.turn = !new_pos.turn;

        (ss+1)->nullmove = false;
        int score = -alphabeta_search(new_pos, info, ss+1, new_pv, -beta, -beta+1, depth-1-R);

        if(score >= beta)
        {
            return score;
        }
    }
    (ss+1)->nullmove = true;
#endif

    PV new_pv;
    Move best_move = NO_MOVE;
    int best_score = -INF;
    Move moves[256];
    int num_moves = movegen(pos, moves);

    // Score moves
    int scores[256] = {0};
    for(int n = 0; n < num_moves; ++n)
    {
        if(moves[n] == tt_move)
        {
            scores[n] = 10001;
        }
#ifdef KILLER_MOVES
        else if(moves[n] == ss->killer)
        {
            scores[n] = 10000;
        }
#endif
        else
        {
            scores[n] = count_captures(pos, moves[n]);

            scores[n] += (is_single(moves[n]) ? 1 : 0);
        }
    }

#ifdef IID
    if(tt_move == NO_MOVE && depth > 5)
    {
        int score = -alphabeta_search(pos, info, ss+1, new_pv, -beta, -alpha, depth-3);

        for(int n = 0; n < num_moves; ++n)
        {
            if(moves[n] == new_pv.moves[0])
            {
                scores[n] = 10001;
                break;
            }
        }
    }
#endif

    int move_num = 0;
    Move move = NO_MOVE;
    while(next_move(moves, num_moves, move, scores))
    {
        assert(move != NO_MOVE);
        Position new_pos = pos;

        makemove(new_pos, move);

        info.nodes++;

#ifdef LMR
        int r = reduction(move_num, depth);
        int score = -alphabeta_search(new_pos, info, ss+1, new_pv, -alpha-1, -alpha, depth-1-r);

        // Re-search
        if(score > alpha)
        {
            score = -alphabeta_search(new_pos, info, ss+1, new_pv, -beta, -alpha, depth-1);
        }
#else
        int score = -alphabeta_search(new_pos, info, ss+1, new_pv, -beta, -alpha, depth-1);
#endif

        if(score > best_score)
        {
            best_move = move;
            best_score = score;
        }

        if(score > alpha)
        {
            alpha = score;

            // Update PV
            pv.moves[0] = move;
            memcpy(pv.moves + 1, new_pv.moves, new_pv.num_moves * sizeof(Move));
            pv.num_moves = new_pv.num_moves + 1;
        }

        if(alpha >= beta)
        {
#ifdef KILLER_MOVES
            if(count_captures(pos, move) == 0)
            {
                ss->killer = move;
            }
#endif
#ifndef NDEBUG
            info.cutoffs[move_num]++;

            if(is_single(move) == true)
            {
                info.single_cutoffs++;
            }
            else
            {
                info.double_cutoffs++;
            }
#endif
            break;
        }

        move_num++;
    }

    if(num_moves == 0)
    {
        int val = score(pos);

        if(val > 0)
        {
            return INF - ss->ply;
        }
        else if(val < 0)
        {
            return -INF + ss->ply;
        }
        else
        {
            return (*info.options).contempt;
        }
    }

    uint8_t flag;
    if(best_score <= alpha_original)
    {
        flag = UPPERBOUND;
    }
    else if(best_score >= beta)
    {
        flag = LOWERBOUND;
    }
    else
    {
        flag = EXACT;
    }

    add(info.tt, key, depth, best_score, best_move, flag);

#ifndef NDEBUG
    Entry testEntry = probe(info.tt, key);
    assert(testEntry.key == key);
    assert(testEntry.depth == depth);
    assert(testEntry.eval == best_score);
    assert(testEntry.move == best_move);
    assert(testEntry.flag == flag);
#endif

    return best_score;
}
