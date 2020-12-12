#ifndef __MAZE_MOVING_OBJECT__
#define __MAZE_MOVING_OBJECT__

#include "Drawable.h"

class MovingObj : public Drawable {
protected:
	float targetX;
	float targetY;
	float x;
	float y;

	float dx;
	float dy;
	float maxDx;
	float maxDy;
	void calculateDxDy();
public:
	MovingObj(float x, float y, float maxDx, float maxDy, float targetX, float targetY);
	virtual void setTarget(float targetX, float targetY);
	virtual void setLocation(float x, float y);
	virtual void move();
	virtual void draw() = 0;
};


#endif // !__MAZE_MOVING_OBJECT__
