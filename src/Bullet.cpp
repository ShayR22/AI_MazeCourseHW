#include "Bullet.hpp"
#include "Drawer.hpp"
#include <iostream>

constexpr auto BULLET_MIN_DAMAGE = 5;

using namespace std;

Bullet::Bullet(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr)
	: Projectile(location, startSpeed, target, boundingDiameter, damage, teamPtr)
{

}

void Bullet::draw()
{
	Drawer::filledCircle(location.x, location.y, boundingDiameter, DrawerColor::RED);
}

float Bullet::calculatePower()
{
	vec2f dist = location - initalLocation;
	float distance = std::max(dist.length(), 1.0f);
	int damagePower = static_cast<int>(damage / distance);
	cout << typeid(*this).name() << " damagePower " << damagePower << endl;

	return static_cast<float>(std::max(damagePower, BULLET_MIN_DAMAGE));
}