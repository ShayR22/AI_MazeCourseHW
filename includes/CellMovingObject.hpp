#ifndef __CELL_MOVING_OBJECT__
#define __CELL_MOVING_OBJECT__

#include "MovingObject.hpp"
#include "BoardCells.hpp"
#include "Cell.hpp"

class CellMovingObject : public MovingObject {
protected:
	vec2i lastPos;
	BoardCells& board;
	BoardCells* nextBoard;

	vec2f getCellCenter(BoardCells& board, Cell& cell);
public:
	CellMovingObject(vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board);

	inline void setLastPos(vec2i& lastPos) { this->lastPos = lastPos; }
	inline vec2i& getLastPos() { return this->lastPos; }

	inline void setBoardCells(BoardCells& board) { this->board = board; }
	inline BoardCells& getBoardCells() { return board; }

	virtual void setTarget(BoardCells& board, Cell &cell);
	virtual void setLocation(BoardCells& board,Cell &cell);
	Cell& getCellLocation();
	Cell& getLastCellLocation();

	virtual void updateLastLocation();
	virtual void draw() = 0;
	virtual void move();
};

#endif