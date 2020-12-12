#include "MovingObj.h"
#include <math.h>
#include <iostream>

using namespace std;

MovingObj::MovingObj(float x, float y, float maxDx, float maxDy, float targetX, float targetY) :
	maxDx(maxDx), maxDy(maxDy)
{
	setLocation(x, y);
	setTarget(targetX, targetY);
}

void MovingObj::calculateDxDy()
{
	float xPart = targetX - x;
	float yPart = targetY - y;
	
	if (xPart == 0 && yPart == 0) {
		dx = 0;
		dy = 0;
		return;
	}

	float magnitude = (float)sqrt(pow(xPart, 2) + pow(yPart, 2));
	float xDir = xPart / magnitude;
	float yDir = yPart / magnitude;

	dx = xDir * maxDx;
	dy = yDir * maxDy;

	cout << "x: " << x << ", y: " << y << endl;
	cout << "tx: " << targetX << ", ty: " << targetY << endl;
	cout << "dx: " << dx << ", dy: " << dy << endl << endl;

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

	calculateDxDy();
}

void MovingObj::move()
{

	//cout << "dx " << dx << ", x " << x << ", targetX " << targetX << ", diff :" << fabs(x - targetX) << endl;
	//cout << "dy " << dy << ", y " << y << ", targetY " << targetY << ", diff :" << fabs(y - targetY) << endl << endl;

	/* TODO look if this if need rework for floating-point arithmetic */
	if (fabs(y - targetY) > fabs(1.5 * dy))
		y += dy;
	if (fabs(x - targetX) > fabs(1.5 * dx))
		x += dx;
}
