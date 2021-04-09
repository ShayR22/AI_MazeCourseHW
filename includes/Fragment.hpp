#ifndef __FRAGMENT__
#define __FRAGMENT__

#include "projectile.hpp"

class Team;

class Fragment : public Projectile {
public:
	Fragment(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr);
	virtual void draw() override;
	virtual float calculatePower();
};

#endif