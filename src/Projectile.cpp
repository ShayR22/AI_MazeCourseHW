#include "Projectile.hpp"
#include "Team.hpp"

unsigned int Projectile::PROJECTILE_ID = 0;

Projectile::Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, Team* teamPtr) 
	: MovingObject(location, startSpeed, target, boundingRadius), damage(damage), teamPtr(teamPtr), id(PROJECTILE_ID++){

}

void Projectile::update() {
	if (!isAtTarget()) {
		move();
	}
}