#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "MovingObject.hpp"
#include <vector>

class Obstacle {
protected:
	int healthPoints;
	int destroyFrame;
	std::vector<Cell*> cover;
public:
	Obstacle(int healthPoints, int destroyFrame, std::vector<Cell*> cover);
	
	inline void setHealthPoints(int healthPoints) { this->healthPoints = healthPoints; }
	inline int getHealthPoints() { return healthPoints; }

	inline void setDestroyFrame(int destoryFrame) { this->destroyFrame = destoryFrame; }
	inline int getDestroyFrame() { return destroyFrame; }

	inline void setCover(std::vector<Cell*>& cover) { this->cover = cover; }
	inline std::vector<Cell*>& getCover() { return cover; }
	
	virtual void draw(float offsetX, float offsetY) = 0;
	virtual bool collision(MovingObject &movingObject) = 0;
};

#endif