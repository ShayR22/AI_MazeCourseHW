#ifndef __PROJECTILE__
#define __PROJECTILE__

#include "MovingObject.hpp"

class Team;

class Projectile : public MovingObject{
protected:
	vec2f initalLocation;
	int damage;
	Team* teamPtr;
	unsigned int id;
	static unsigned int PROJECTILE_ID;
	bool requireCreation;
	/* if projectile can spawn other projectiles this flag would force spawn upon death */
	bool createOnDeath;

public:
	Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr);
	virtual void update();
	virtual void draw() = 0;
	virtual void creation();

	float calculatePower();

	inline int getDamage() { return damage; }
	inline void setDamage(int damage) { this->damage = damage; };

	inline vec2f& getInitalLocation() { return initalLocation; }
	inline void setInitalLocation(vec2f& initalLocation) { this->initalLocation = initalLocation; }

	inline bool getRequireCeation() { return requireCreation; }
	inline void setRequireCreation(bool requireCreation) { this->requireCreation = requireCreation; };

	inline bool getCreateOnDeath() { return createOnDeath; }
	inline void setCreateOnDeath(bool createOnDeath) { this->createOnDeath = createOnDeath; }

};

#endif