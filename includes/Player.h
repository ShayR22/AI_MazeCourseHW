#ifndef __PACKMAN__
#define __PACKMAN__

#include "MazeMovingObj.h"
#include "Maze.h"

class Player : public MazeMovingObj {

public:
	Player(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY);
	virtual void draw() override;
};


#endif // !__PACKMAN__
