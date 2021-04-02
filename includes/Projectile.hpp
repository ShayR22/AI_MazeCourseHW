#ifndef __PROJECTILE__
#define __PROJECTILE__

#include "MovingObject.hpp"

class Team;

class Projectile : public MovingObject{
protected:
	int damage;
	Team* teamPtr;
	unsigned int id;
	static unsigned int PROJECTILE_ID;

public:
	Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, Team* teamPtr);
	virtual void update();
	virtual void draw() = 0;

	inline int getDamage() { return damage; }
	inline void setDamage(int damage) { this->damage = damage; };
};

#endif