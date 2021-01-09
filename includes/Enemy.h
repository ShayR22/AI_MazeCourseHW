#ifndef __ENEMY__
#define __ENEMY__

#include <chrono>
#include "MazeMovingObj.h"
#include "Maze.h"

class Enemy : public MazeMovingObj {
private:
	static const int MIN_BRAIN_CALC_TIMEOUT_MS = 4000;
	static const int MAX_BRAIN_CALC_TIMEOUT_MS = 10000;

	std::chrono::high_resolution_clock::time_point lastTick;
	int	brainCalculationEachMs;
	int lastCellX;
	int lastCellY;

	static inline int generate_random_brain_calc() { return MIN_BRAIN_CALC_TIMEOUT_MS + (rand() % (MAX_BRAIN_CALC_TIMEOUT_MS - MIN_BRAIN_CALC_TIMEOUT_MS)); }
public:
	Enemy(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int brainCalculationEachMs=generate_random_brain_calc());
	virtual void draw() override;
	virtual void move();

	inline std::chrono::high_resolution_clock::time_point getLastTick() { return lastTick; }
	inline void setLastTick(std::chrono::high_resolution_clock::time_point lt) { lastTick = lt; }

	inline int getBrainCalculationEachMs() { return brainCalculationEachMs; }
	inline void setBrainCalculationEachMs(int bcem) { brainCalculationEachMs = bcem; }
};


#endif // !__ENEMY__
