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

	bool atTarget;
	void calculateDxDy();
public:
	MovingObj(float x, float y, float maxDx, float maxDy, float targetX, float targetY);
	virtual void setTarget(float targetX, float targetY);
	virtual void setLocation(float x, float y);
	virtual void move();
	virtual void draw() = 0;
	inline bool isAtTarget() { return atTarget; }
	inline float getX() { return x; }
	inline float getY() { return y; }
};


#endif // !__MAZE_MOVING_OBJECT__
