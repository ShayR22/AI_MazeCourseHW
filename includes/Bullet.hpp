#ifndef __BULLET__
#define __BULLET__

#include "Projectile.hpp"

class Bullet : public Projectile {
public:
	Bullet(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr);
	virtual void draw() override;
	virtual float calculatePower();
};

#endif