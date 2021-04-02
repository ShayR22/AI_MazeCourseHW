#include "Grenade.hpp"
#include "Game.hpp"
#include "Team.hpp"
#include <time.h>
#include "Fragment.hpp"
#include <math.h>
#include "Drawer.hpp"
#include "CollisionLogic.hpp"

Grenade::Grenade(vec2f location, vec2f startSpeed, vec2f target, float boundingRadius, int damage, int explosionTimeoutMS, int numFragments, Team *teamPtr)
	: Projectile(location, startSpeed, target, boundingRadius, damage, teamPtr), explosionTimeoutMS(explosionTimeoutMS), 
	numFragments(numFragments), startTime(std::chrono::high_resolution_clock::now()), wasAlreadyExplode(false)
{

}

void Grenade::update()
{
	if (wasAlreadyExplode) {
		return;
	}

	if (explosionTimeoutPassed()) {
		setTarget(location);
		registerFragments();
		wasAlreadyExplode = true;
		return;
	}

	Projectile::update();
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
	cout << angleDelta << endl;
	for (int angle = 0; angle < 360; angle += angleDelta) {
		vec2f direction((float)cos(angle * M_PI / 180.), (float)sin(angle * M_PI / 180.));
		// cout << "(" << (float)cos(angle * M_PI / 180.) << ", " << (float)sin(angle * M_PI / 180.) << ")" << endl;
		vec2f myLocation = location;
		vec2f mySpeed = maxSpeed;
		vec2f myTarget;

		calcTarget(myLocation, direction, myTarget);
		Fragment *f = new Fragment(myLocation, mySpeed, myTarget, boundingRadius / 2.0f, damage, teamPtr);
		teamPtr->registerProjectile(*f);
		// registerSingleFragment(speed);
	}
}

void Grenade::calcTarget(vec2f& location, vec2f& speed, vec2f& myTarget)
{
	/*std::vector<vec2f> points;
	Game* theGame = Game::getInstance();
	Room* room = theGame->getRoom(location);
	if (!room)
		return;

	float xmin = room->getXYOffset().x;
	float xmax = xmin + room->getWidth();
	float ymin = room->getXYOffset().y;
	float ymax = ymin + room->getHeight();

	points.push_back(vec2f(xmin, ymin));
	points.push_back(vec2f(xmax, ymin));
	points.push_back(vec2f(xmax, ymax));
	points.push_back(vec2f(xmin, ymax));

	room->getShape(points);

	vec2f collisionResult = CollisionLogic::getCollision(location, speed, points);
	myTarget.set(collisionResult.x, collisionResult.y);*/
	Game* theGame = Game::getInstance();
	Room* room = theGame->getRoom(location);
	vec2f locationRelate2Room = location - room->getXYOffset();

	// Note: the calculation is based on "room' system' axis"
	vec2f collisionResult = CollisionLogic::calcCollision(room, locationRelate2Room, speed);
	myTarget.set(room->getXYOffset().x + collisionResult.x, room->getXYOffset().y + collisionResult.y);
}


void Grenade::dummyCalcTarget(vec2f& location, vec2f& speed, vec2f& myTarget)
{
	vec2f dummyDelta = (speed * 500);
	myTarget = location + dummyDelta;
}

void Grenade::registerSingleFragment(vec2f speed)
{
	// TODO: Create a fragment and register to team
}

void Grenade::draw()
{
	if (!wasAlreadyExplode) {
		Drawer::filledCircle(getLocation().x, getLocation().y, getBoundingRadius() * 2, DrawerColor::RED);
		Drawer::filledCircle(getLocation().x, getLocation().y, getBoundingRadius(), DrawerColor::BLACK);
	}
}

