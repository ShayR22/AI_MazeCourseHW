#ifndef __MAZE_MOVING_OBJ__
#define __MAZE_MOVING_OBJ__

#include "Maze.h"
#include "Cell.h"
#include "MovingObj.h"

class MazeMovingObj : public MovingObj {
protected:
	cell_mat& cells;
	void getOpenGLCellCenter(Cell& cell, float* xy);
	int lastCellX;
	int lastCellY;
public:
	MazeMovingObj(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY);
	virtual void setTarget(Cell& cell);
	virtual void setLocation(Cell& cell);
	virtual void draw() = 0;
	inline Cell& getCellLocation() { return cells[(int)x][(int)y]; }
	inline Cell& getLastCellLoctaion() { return cells[lastCellX][lastCellY]; }
	virtual void updateLastLocation();

};


#endif // !__MAZE_MOVING_OBJ__
