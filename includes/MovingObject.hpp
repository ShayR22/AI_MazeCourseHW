#ifndef __MOVING_OBJECT__
#define __MOVING_OBJECT__

class MovingObject {
protected:
	float x;
	float y;
	float dx;
	float dy;
	float maxDx;
	float maxDy;
	float targetX;
	float targetY;
	float boundingRadius;
public:
	MovingObject(float x, float y, float dx, float fy, float maxDx, float maxDy,
				 float targetX, float targetY, float boundingRadius);
	
	inline void setX(float x) { this->x = x; }
	inline float getX() { return x; }
	inline void setY(float y) { this->y = y; }
	inline float getY() { return y; }
	inline void setDx(float dx) { this->dx = dx; }
	inline float getDx() { return dx; }
	inline void setDy(float dy) { this->dy = dy; }
	inline float getDy() { return dy; }
	inline void setMaxDx(float maxDx) { this->maxDx = maxDx; }
	inline float getMaxDx() { return maxDx; }
	inline void setMaxDy(float maxDy) { this->maxDy = maxDy; }
	inline float getMaxDy() { return maxDy; }
	inline void setTargetX(float targetX) {this->targetX = targetX; }
	inline float getTargetX() { return targetX; }
	inline void setTargetY(float targetY) { this->targetY = targetY; }
	inline float getTargetY() { return targetY; }
	inline void setBoundingRadius(float boundingRadius) { this->boundingRadius = boundingRadius; }
	inline float getBoundingRadius() { return boundingRadius; }
	
	void setTarget(float targetX, float targetY);
	void setLocation(float x, float y);
	bool isAtTarget();

	virtual void draw() = 0;
	virtual void move() = 0;
};

#endif