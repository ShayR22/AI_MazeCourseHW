#ifndef __ENEMY__
#define __ENEMY__

#include "MazeMovingObj.h"
#include "Maze.h"

class Enemy : public MazeMovingObj {
private:
	int lastCellX;
	int lastCellY;
public:
	Enemy(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY);
	virtual void draw() override;
	virtual void move();
};


#endif // !__ENEMY__
