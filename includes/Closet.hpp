#ifndef __CLOSET__
#define __CLOSET__

#include "Obstacle.hpp"
#include "MovingObject.hpp"
#include "Cell.hpp"
#include <vector>

class Closet : public Obstacle {

public:
	Closet(int healthPoints, int destroyFrame, std::vector<Cell*> cover);

	virtual void draw(float offsetX, float offsetY);
	virtual bool collision(MovingObject& movingObject);
};

#endif