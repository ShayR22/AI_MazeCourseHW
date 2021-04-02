#ifndef __GAME_POINT__
#define __GAME_POINT__

#include "BoardCells.hpp"
#include "Cell.hpp"

class GamePoint {
public:
	GamePoint(BoardCells* board, Cell* cell) : board(board), cell(cell) {}
	BoardCells* board;
	Cell* cell;

	inline float getAbsoluteX() const { return board->getXYOffset().x + cell->getX(); }
	inline float getAbsoluteY() const { return board->getXYOffset().y + cell->getY(); }

	friend bool operator < (const GamePoint& gp1, const GamePoint& gp2)
	{
		return gp1.getAbsoluteX() > gp2.getAbsoluteX() && gp1.getAbsoluteY() > gp2.getAbsoluteY();
	}

	friend bool operator==(const GamePoint& gp1, const GamePoint& gp2)
	{
		return gp1.getAbsoluteX() == gp2.getAbsoluteX() && gp1.getAbsoluteY() == gp2.getAbsoluteY();
	}
};

#endif
