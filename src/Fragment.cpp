#include "Fragment.hpp"
#include "Drawer.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;

Fragment::Fragment(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, Team* teamPtr)
	: Projectile(location, startSpeed, target, boundingDiameter, damage, teamPtr)
{

}

void Fragment::draw()
{
	Drawer::filledCircle(location.x, location.y, boundingDiameter, DrawerColor::BLACK);
}

float Fragment::calculatePower()
{
	float speedMagnitude = speed.length();
	float speedPower = speedMagnitude * 10;
	float power = std::min(std::max(speedPower, 1.0f), 10.0f);
	return power;
}




