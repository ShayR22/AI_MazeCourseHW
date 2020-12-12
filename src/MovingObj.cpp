#include "MovingObj.h"
#include <math.h>


MovingObj::MovingObj(float x, float y, float maxDx, float maxDy, float targetX, float targetY) :
	x(x), y(y), maxDx(maxDx), maxDy(maxDy)
{
	setTarget(targetX, targetY);
}


void MovingObj::calculateDxDy()
{
	float xPart = targetX - x;
	float yPart = targetY - y;

	float magnitude = sqrt(pow(xPart, 2) + pow(yPart, 2));
	float xDir = x / magnitude;
	float yDir = y / magnitude;

	dx = xDir * maxDx;
	dy = xDir * maxDy;
}


void MovingObj::setTarget(float x, float y)
{
	targetX = x;
	targetY = y;

	calculateDxDy();
}

void MovingObj::setLocation(float x, float y)
{
	this->x = x;
	this->y = y;
}

void MovingObj::move()
{
	/* TODO look if this if need rework for floating-point arithmetic */
	if (x != targetX)
		x += dx;
	if (y != targetY)
		y += dy;
}