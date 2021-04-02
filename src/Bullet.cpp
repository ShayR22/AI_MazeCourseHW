#include "Bullet.hpp"
#include "Drawer.hpp"

Bullet::Bullet(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, Team* teamPtr)
	: Projectile(location, startSpeed, target, boundingRadius, damage, teamPtr)
{

}

void Bullet::draw()
{
	Drawer::filledCircle(location.x, location.y, boundingRadius * 2, DrawerColor::RED);
	// Drawer::rect(getLocation().x, getLocation().y, getBoundingRadius() * 2, getBoundingRadius() * 2, DrawerColor::RED);
}
