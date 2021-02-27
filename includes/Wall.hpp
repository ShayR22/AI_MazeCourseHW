#ifndef __WALL__
#define __WALL__

#include "Obstacle.hpp"
#include "MovingObject.hpp"
#include "Cell.hpp"
#include <vector>

class Wall : public Obstacle {
public:
	Wall(int healthPoints, int destroyFrame, std::vector<Cell*> cover);

	virtual void draw(float offsetX, float offsetY);
	virtual bool collision(MovingObject& movingObject);
};

#endif