#include "Projectile.hpp"
#include "Team.hpp"

unsigned int Projectile::PROJECTILE_ID = 0;

using namespace std;

Projectile::Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr) 
	: MovingObject(location, startSpeed, target, boundingDiameter), damage(damage), teamPtr(teamPtr), id(PROJECTILE_ID++),
	initalLocation(location), requireCreation(false), createOnDeath(false)
{

}

void Projectile::creation()
{
	/* DO NOTHING 
	 * should be override if required by drived class
	 */
}

void Projectile::update() {
	if (!isAtTarget()) {
		move();
	}
}