#include "MovingObject.hpp"

void MovingObject::calculateDxDy()
{
    vec2f relativePath = location - target;

    if (relativePath.x == 0 && relativePath.y == 0) {
        speed.set(0, 0);
        return;
    }

    relativePath.normalize();
    speed *= relativePath;
}

MovingObject::MovingObject(vec2f location,vec2f maxSpeed, vec2f target, float boundingRadius) :
	location(location), maxSpeed(maxSpeed), target(target), boundingRadius(boundingRadius)
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
    vec2f diff(location);
    diff -= target;

    if (fabs(diff.x) > fabs(1.5 * speed.x))
        location.x += speed.x;
    if (fabs(diff.y) > fabs(1.5 * speed.y))
        location.y += speed.y;

    atTarget = (location.x == lastLocation.x) && (location.y == lastLocation.y);
}



