#include "Projectile.hpp"
#include "Team.hpp"

unsigned int Projectile::PROJECTILE_ID = 0;

using namespace std;

constexpr auto BULLET_MIN_DAMAGE = 5;

Projectile::Projectile(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr) 
	: MovingObject(location, startSpeed, target, boundingDiameter), damage(damage), teamPtr(teamPtr), id(PROJECTILE_ID++),
	initalLocation(location), requireCreation(false), createOnDeath(false)
{

}

float Projectile::calculatePower()
{
	vec2f dist = location - initalLocation;
	float distance = std::max(dist.length(), 1.0f);
	int damagePower = static_cast<int>(damage / distance);
	return static_cast<float>(std::max(damagePower, BULLET_MIN_DAMAGE));
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