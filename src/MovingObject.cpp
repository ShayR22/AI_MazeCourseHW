#include "MovingObject.hpp"

void MovingObject::calculateDxDy()
{
    vec2f relativePath = target - location;

    if (relativePath.x == 0 && relativePath.y == 0) {
        speed.set(0, 0);
        return;
    }

    relativePath.normalize();
    speed = maxSpeed * relativePath;
}

MovingObject::MovingObject(vec2f location,vec2f maxSpeed, vec2f target, float boundingDiameter) :
	location(location), maxSpeed(maxSpeed), target(target), boundingDiameter(boundingDiameter)
{
    calculateDxDy();
}

void MovingObject::setTarget(vec2f target)
{
    this->target = target;
    calculateDxDy();
}

void MovingObject::setLocation(vec2f location)
{
    this->location = location;
    calculateDxDy();
}

void MovingObject::move()
{
    vec2f lastLocation(location.x, location.y);
    vec2f diff = target - location;

    if (fabs(diff.x) > fabs(speed.x)) {
        location.x += speed.x;
    }
    else {
        location.x += diff.x;
    }
    if (fabs(diff.y) > fabs(speed.y)) {
        location.y += speed.y;
    }
    else {
        location.y += diff.y;
    }

    atTarget = ((fabs(diff.x) < 0.001) && (fabs(diff.y) < 0.001));
}



