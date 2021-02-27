#ifndef __BULLET__
#define __BULLET__

#include "Projectile.hpp"

class Bullet : public Projectile {
public:
	Bullet(int damage);
	virtual void draw();
};

#endif