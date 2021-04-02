#include "Fragment.hpp"
#include "Drawer.hpp"
#include <stdio.h>

Fragment::Fragment(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, Team* teamPtr)
	: Projectile(location, startSpeed, target, boundingRadius, damage, teamPtr)
{

}

void Fragment::draw()
{
	// printf("x: %lf, y: %lf\n", location.x, location.y);
	Drawer::filledCircle(location.x, location.y, boundingRadius * 2, DrawerColor::BLACK);
	// Drawer::rect(getLocation().x, getLocation().y, getBoundingRadius() * 2, getBoundingRadius() * 2, DrawerColor::RED);
}



