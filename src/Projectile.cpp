#include "Projectile.hpp"
#include "Team.hpp"

unsigned int Projectile::PROJECTILE_ID = 0;

Projectile::Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, Team* teamPtr) 
	: MovingObject(location, startSpeed, target, boundingRadius), damage(damage), teamPtr(teamPtr), id(PROJECTILE_ID++),
	initalLocation(location)
{

}

float Projectile::calculatePower()
{
	vec2f dist = location - initalLocation;
	float distance = std::max(dist.length(), 1.0f);
	return damage / distance;
}


void Projectile::update() {
	if (!isAtTarget()) {
		move();
	}
}