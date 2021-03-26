#ifndef __CELL_MOVING_OBJECT__
#define __CELL_MOVING_OBJECT__

#include "MovingObject.hpp"
#include "BoardCells.hpp"
#include "Cell.hpp"

class CellMovingObject : public MovingObject {
protected:
	vec2i lastPos;
	BoardCells& board;
private:
	vec2f getCellCenter(Cell& cell) { return vec2f(cell.getX() + 0.5f, cell.getY() + 0.5f); }
public:
	CellMovingObject(vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board);

	inline void setLastPos(vec2i& lastPos) { this->lastPos = lastPos; }
	inline vec2i& getLastPos() { return this->lastPos; }

	inline void setBoardCells(BoardCells& board) { this->board = board; }
	inline BoardCells& getBoardCells() { return board; }

	virtual void setTarget(Cell &cell);
	virtual void setLocation(Cell &cell);
	inline Cell& getCellLocation();
	inline Cell& getLastCellLocation();

	virtual void updateLastLocation();
	virtual void draw() = 0;
	virtual void move() = 0;
};

#endif