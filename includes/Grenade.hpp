#ifndef __GRENADE__
#define __GRENADE__

#include "projectile.hpp"

class Grenade : public Projectile {
private:
	int explosionTimeoutMS;
	int numFragments;
public:
	Grenade(int damage, int exploasionTimeoutMS, int numFragments);
	
	inline void setExplosionTimeoutMS(int exploasionTimeoutMS) { this->explosionTimeoutMS = exploasionTimeoutMS; }
	inline int getExplosionTimeoutMS() { return explosionTimeoutMS; }
	inline void setNumFragments(int numFragments) { this->numFragments = numFragments; }
	inline int getNumFragments() { return numFragments; }

	virtual void draw();
	virtual void update();
};

#endif