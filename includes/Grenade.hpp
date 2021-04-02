#ifndef __GRENADE__
#define __GRENADE__

#include "projectile.hpp"
#include <chrono>

class Grenade : public Projectile {
private:
	int explosionTimeoutMS;
	int numFragments;
	std::chrono::high_resolution_clock::time_point startTime;
	bool wasAlreadyExplode;

public:
	Grenade(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, int exploasionTimeoutMS, int numFragments, Team* teamPtr);
	
	inline void setExplosionTimeoutMS(int exploasionTimeoutMS) { this->explosionTimeoutMS = exploasionTimeoutMS; }
	inline int getExplosionTimeoutMS() { return explosionTimeoutMS; }
	inline void setNumFragments(int numFragments) { this->numFragments = numFragments; }
	inline int getNumFragments() { return numFragments; }

	virtual void draw() override;
	virtual void update() override;

private:
	bool explosionTimeoutPassed();
	void registerFragments();
	void registerSingleFragment(vec2f speed);
	void calcTarget(vec2f& location, vec2f& speed, vec2f& myTarget);
	void dummyCalcTarget(vec2f& location, vec2f& speed, vec2f& myTarget);
};

#endif