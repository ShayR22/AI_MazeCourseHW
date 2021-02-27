#ifndef __CELL_MOVING_OBJECT__
#define __CELL_MOVING_OBJECT__

#include "MovingObject.hpp"
#include "BoardCells.hpp"
#include "Cell.hpp"

class CellMovingObject : public MovingObject {
protected:
	int lastX;
	int lastY;
	BoardCells& board;
public:
	CellMovingObject(float x, float y, float dx, float fy, float maxDx, float maxDy,
		float targetX, float targetY, float boundingRadius, BoardCells& board);

	inline void setLastX(int lastX) { this->lastX = lastX; }
	inline int getLastX() { return lastX; }
	inline void setLastY(int lastY) { this->lastY = lastY; }
	inline int getLastY() { return lastY; }
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