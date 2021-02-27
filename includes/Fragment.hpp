#ifndef __FRAGMENT__
#define __FRAGMENT__

#include "projectile.hpp"

class Fragment : public Projectile {
public:
	Fragment(int damage);
	virtual void draw();
};

#endif