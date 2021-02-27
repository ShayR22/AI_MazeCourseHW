#ifndef __PROJECTILE__
#define __PROJECTILE__

class Projectile {
private:
	int damage;
public:
	virtual void update();
	virtual void draw();

	inline int getDamage() { return damage; }
	inline void setDamage(int damage) { this->damage = damage; };
};

#endif