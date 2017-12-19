#include "phase.hpp"
#include "other.hpp"

float phase(const Position& pos)
{
	return (float)(popcountll(pos.pieces[PIECE::CROSS]) + popcountll(pos.pieces[PIECE::NOUGHT])) / (49 - popcountll(pos.blockers));
}

bool isEndgame(const Position& pos)
{
	return phase(pos) >= 0.75;
}