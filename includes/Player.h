#ifndef __PLAYER__
#define __PLAYER__

#include "MazeMovingObj.h"
#include "Maze.h"

class Player : public MazeMovingObj {
private:
	int* numCoins;
	int lastCellX;
	int lastCellY;
public:
	Player(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int* numCoins);
	virtual void draw() override;
	virtual void move();
};


#endif // !__PLAYER__
