#ifndef __MOVING_OBJECT__
#define __MOVING_OBJECT__

#include "vec2.h"

class MovingObject {
private:
	void calculateDxDy();
protected:
	bool atTarget;
	vec2f location;
	vec2f speed;
	vec2f maxSpeed;
	vec2f target;
	float boundingDiameter;
public:
	MovingObject(vec2f location, vec2f maxSpeed, vec2f target, float boundingDiameter);

	void setTarget(vec2f target);
	void setLocation(vec2f location);

	inline vec2f getLocation() { return this->location; }

	inline float getX() { return location.x; }
	inline float getY() { return location.y; }

	inline void setSpeed(vec2f speed) { this->speed = speed; }
	inline vec2f getSpeed() { return this->speed; }

	inline void setMaxSpeed(vec2f maxSpeed) { this->maxSpeed = maxSpeed; }
	inline vec2f getMaxSpeed() { return this->maxSpeed; }

	inline vec2f getTarget() { return this->target; }

	inline void setBoundingDiameter(float boundingDiameter) { this->boundingDiameter = boundingDiameter; }
	inline float getBoundingDiameter() { return boundingDiameter; }

	inline bool isAtTarget() { return this->atTarget; }

	virtual void move();
	virtual void draw() = 0;
};

#endif