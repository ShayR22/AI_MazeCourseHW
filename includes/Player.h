#ifndef __PLAYER__
#define __PLAYER__

#include "MazeMovingObj.h"
#include "Maze.h"

class Player : public MazeMovingObj {
private:
	int& numCoins;
	int lastCellX;
	int lastCellY;
	float enemySearchRadius;
public:
	Player(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int& numCoins);
	virtual void draw() override;
	virtual void move();

	inline float getEnemySearchRadius() { return enemySearchRadius; }
	inline void setEnemySearchRadius(float r) { enemySearchRadius = r; }

	inline Cell& getLastPlayerLoctaion() { return cells[lastCellX][lastCellY]; }
};


#endif // !__PLAYER__
