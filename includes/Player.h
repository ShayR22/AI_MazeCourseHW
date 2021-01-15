#ifndef __PLAYER__
#define __PLAYER__

#include <chrono>
#include "MazeMovingObj.h"
#include "Maze.h"

class Player : public MazeMovingObj {
private:
	int& numCoins;
	float enemySearchRadius;
	std::chrono::high_resolution_clock::time_point lastTick;
public:
	Player(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int& numCoins);
	virtual void draw() override;

	virtual void updateLastLocation();

	inline std::chrono::high_resolution_clock::time_point getLastTick() { return lastTick; }
	inline void setLastTick(std::chrono::high_resolution_clock::time_point lt) { lastTick = lt; }

	inline float getEnemySearchRadius() { return enemySearchRadius; }
	inline void setEnemySearchRadius(float r) { enemySearchRadius = r; }
};


#endif // !__PLAYER__
