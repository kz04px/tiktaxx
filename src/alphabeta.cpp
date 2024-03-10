#include <cassert>
#include <cstring>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <limits>
#include "eval.hpp"
#include "hashtable.hpp"
#include "next-move.hpp"
#include "other.hpp"
#include "pv.hpp"
#include "score.hpp"
#include "search.hpp"
#include "searchinfo.hpp"
#include "searchstack.hpp"
#include "sorting.hpp"

int reduction(const int move_num, const int depth) {
    assert(move_num >= 0);
    assert(depth >= 0);

    if (move_num < 2 || depth < 3) {
        return 0;
    }

    if (move_num < 6) {
        return 1;
    } else if (move_num < 12) {
        return depth / 3;
    } else {
        return depth / 2;
    }
}

int alphabeta_search(const libataxx::Position &pos,
                     search_info &info,
                     search_stack *ss,
                     PV &pv,
                     int alpha,
                     int beta,
                     int depth) {
    assert(ss != NULL);
    assert(depth >= 0);
    assert(beta >= alpha);

    if (pos.is_gameover()) {
        const auto material = pos.get_us().count() - pos.get_them().count();
        if (material > 0) {
            return INF - ss->ply;
        } else if (material < 0) {
            return -INF + ss->ply;
        } else {
            return (*info.options).contempt;
        }
    }

    if (depth == 0 || info.depth >= MAX_DEPTH) {
        info.leaf_nodes++;
        return eval(pos);
    }

    if (info.nodes != 0) {
        // Stop searching if we've ran out of time
        if (*info.stop == true || clock() >= info.end) {
            return 0;
        }

        // Send an update on what we're doing
        if (info.nodes % 2000000 == 0) {
            double time_spent = (double)(clock() - info.start) / CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes / time_spent)
                      << std::endl;
        }
    }

    const int alpha_original = alpha;
    libataxx::Move tt_move = libataxx::Move::nullmove();
    const bool pvnode = (beta - alpha == 1);

    if (info.tt) {
        // Check the hash table
        Entry entry = probe(info.tt, pos.get_hash());
        if (pos.get_hash() == entry.key) {
            tt_move = get_move(entry);

#ifndef NDEBUG
            info.hash_hits++;
            if (pos.is_legal_move(tt_move) == false) {
                info.hash_collisions++;
            }
#endif

            if (get_depth(entry) >= depth &&
                pos.is_legal_move(tt_move) == true) {
                const int score = eval_from_tt(get_eval(entry), ss->ply);

                assert(score <= INF);
                assert(score >= -INF);

                switch (get_flag(entry)) {
                    case EXACT:
                        pv.num_moves = 1;
                        pv.moves[0] = tt_move;
                        return get_eval(entry);
                    case LOWERBOUND:
                        alpha = (alpha > score ? alpha : score);
                        break;
                    case UPPERBOUND:
                        beta = (beta < score ? beta : score);
                        break;
                    default:
                        assert(false);
                        break;
                }

                if (alpha >= beta) {
                    pv.num_moves = 1;
                    pv.moves[0] = tt_move;
                    return score;
                }
            }
        }
    }

    PV new_pv;

#ifdef NULLMOVE
#define R (2)

    if (ss->ply > 0 && ss->nullmove && depth > 2 && !pvnode) {
        new_pv.num_moves = 0;
        libataxx::Position new_pos = pos;
        new_pos.makemove(libataxx::Move::nullmove());

        (ss + 1)->nullmove = false;
        int score = -alphabeta_search(
            new_pos, info, ss + 1, new_pv, -beta, -beta + 1, depth - 1 - R);

        if (score >= beta) {
            return score;
        }
    }
    (ss + 1)->nullmove = true;
#endif

    libataxx::Move best_move = libataxx::Move::nullmove();
    int best_score = std::numeric_limits<int>::lowest();
    libataxx::Move moves[256];
    const int num_moves = pos.legal_moves(moves);

    // Score moves
    int scores[256] = {0};
    for (int n = 0; n < num_moves; ++n) {
        if (moves[n] == tt_move) {
            scores[n] = 10001;
        }
#ifdef KILLER_MOVES
        else if (moves[n] == ss->killer) {
            scores[n] = 10000;
        }
#endif
        else {
            scores[n] = count_captures(pos, moves[n]);

            scores[n] += (moves[n].is_single() ? 1 : 0);
        }
    }

#ifdef IID
    if (tt_move == libataxx::Move::nullmove() && depth > 5) {
        new_pv.num_moves = 0;
        int score = -alphabeta_search(
            pos, info, ss + 1, new_pv, -beta, -alpha, depth - 3);

        for (int n = 0; n < num_moves; ++n) {
            if (moves[n] == new_pv.moves[0]) {
                scores[n] = 10001;
                break;
            }
        }
    }
#endif

    int move_num = 0;
    libataxx::Move move = libataxx::Move::nullmove();
    while (next_move(moves, num_moves, move, scores)) {
        new_pv.num_moves = 0;
        libataxx::Position new_pos = pos;

        new_pos.makemove(move);

        info.nodes++;

#ifdef FUTILITY_PRUNING
        int material =
            100 * (new_pos.get_us().count() - new_pos.get_them().count());
        if (move_num > 0 && depth < 3 && -material + 100 < alpha) {
            continue;
        }
#endif

#ifdef LMR
        const int r = reduction(move_num, depth);
        int score = -alphabeta_search(
            new_pos, info, ss + 1, new_pv, -alpha - 1, -alpha, depth - 1 - r);

        // Re-search
        if (score > alpha) {
            new_pv.num_moves = 0;
            score = -alphabeta_search(
                new_pos, info, ss + 1, new_pv, -beta, -alpha, depth - 1);
        }
#else
        const int score = -alphabeta_search(
            new_pos, info, ss + 1, new_pv, -beta, -alpha, depth - 1);
#endif

        assert(score <= INF);
        assert(score >= -INF);

        if (score > best_score) {
            best_move = move;
            best_score = score;
        }

        if (score > alpha) {
            alpha = score;

            // Update PV
            pv.moves[0] = move;
            memcpy(pv.moves + 1,
                   new_pv.moves,
                   new_pv.num_moves * sizeof(libataxx::Move));
            pv.num_moves = new_pv.num_moves + 1;
        }

        if (alpha >= beta) {
#ifdef KILLER_MOVES
            if (count_captures(pos, move) == 0) {
                ss->killer = move;
            }
#endif
#ifndef NDEBUG
            info.cutoffs[move_num]++;

            int num_captured = count_captures(pos, move);
            info.capture_cutoffs[num_captured]++;

            if (move.is_single() == true) {
                info.single_cutoffs++;
            } else {
                info.double_cutoffs++;
            }
#endif
            break;
        }

        move_num++;
    }

    assert(best_score != std::numeric_limits<int>::lowest());

    // Every move failed low so we need to update the pv
    if (pv.num_moves == 0) {
        assert(best_score <= alpha);

        pv.num_moves = 1;
        pv.moves[0] = best_move;
    }

    if (info.tt) {
        uint8_t flag;
        if (best_score <= alpha_original) {
            flag = UPPERBOUND;
        } else if (best_score >= beta) {
            flag = LOWERBOUND;
        } else {
            flag = EXACT;
        }

        add(info.tt,
            pos.get_hash(),
            depth,
            eval_to_tt(best_score, ss->ply),
            best_move,
            flag);

#ifndef NDEBUG
        Entry test_entry = probe(info.tt, pos.get_hash());
        assert(test_entry.key == pos.get_hash());
        assert(test_entry.depth == depth);
        assert(eval_from_tt(test_entry.eval, ss->ply) == best_score);
        assert(test_entry.move == best_move);
        assert(test_entry.flag == flag);
#endif
    }

    return best_score;
}
