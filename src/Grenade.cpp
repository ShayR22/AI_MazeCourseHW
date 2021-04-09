#include "Grenade.hpp"
#include "Game.hpp"
#include "Team.hpp"
#include <time.h>
#include "Fragment.hpp"
#include <math.h>
#include "Drawer.hpp"
#include "CollisionLogic.hpp"

Grenade::Grenade(vec2f location, vec2f startSpeed, vec2f target, float boundingDiameter, int damage, int explosionTimeoutMS, int numFragments, Team *teamPtr)
	: Projectile(location, startSpeed, target, boundingDiameter, damage, teamPtr), explosionTimeoutMS(explosionTimeoutMS), 
	numFragments(numFragments), startTime(std::chrono::high_resolution_clock::now()), wasAlreadyExplode(false)
{
	createOnDeath = true;
}

void Grenade::update()
{
	if (wasAlreadyExplode) {
		return;
	}

	Projectile::update();

	if (explosionTimeoutPassed() || isAtTarget()) {
		requireCreation = true;
		return;
	}
}

void Grenade::creation()
{
	setTarget(location);
	registerFragments();
	wasAlreadyExplode = true;
	requireCreation = false;
}


bool Grenade::explosionTimeoutPassed()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> diffTime = currentTime - startTime;

	return (diffTime.count() > explosionTimeoutMS);
}

#include <iostream>
using namespace std;

void Grenade::registerFragments()
{
	int angleDelta = (int)(360. / numFragments);
	for (int angle = 0; angle < 360; angle += angleDelta) {
		vec2f direction((float)cos(angle * M_PI / 180.), (float)sin(angle * M_PI / 180.));
		vec2f myLocation = location;
		/* generate rand speed between 0.15 - 0.29 for creation good spread*/
		float randSpeed = ((rand() % 15) + 15) / 100.0f;

		vec2f mySpeed(randSpeed, randSpeed);
		vec2f myTarget;

		calcTarget(myLocation, direction, myTarget);
		Fragment *f = new Fragment(myLocation, mySpeed, myTarget, boundingDiameter / 2.0f, damage, teamPtr);
		teamPtr->registerProjectile(*f);
	}
}

void Grenade::calcTarget(vec2f& location, vec2f& speed, vec2f& myTarget)
{
	Game* theGame = Game::getInstance();
	Room* room = theGame->getRoom(location);
	vec2f locationRelate2Room = location - room->getXYOffset();

	// Note: the calculation is based on "room' system' axis"
	vec2f collisionResult = CollisionLogic::calcCollision(room, locationRelate2Room, speed);
	myTarget.set(room->getXYOffset().x + collisionResult.x, room->getXYOffset().y + collisionResult.y);
}

void Grenade::draw()
{
	if (!wasAlreadyExplode) {
		Drawer::filledCircle(getLocation().x, getLocation().y, getBoundingDiameter() * 2, DrawerColor::RED);
		Drawer::filledCircle(getLocation().x, getLocation().y, getBoundingDiameter(), DrawerColor::BLACK);
	}
}

