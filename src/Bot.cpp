#include "Bot.hpp"
#include "CollisionLogic.hpp"
#include "Drawer.hpp"
#include "Cell.hpp"
#include "GamePoint.hpp"
#include "Bullet.hpp"
#include "Grenade.hpp"
#include "SupportBot.hpp"
#include "heruistics.h"
#include <iostream>
#include <stack>
#include <map>

using namespace std;

constexpr auto GRENADE_TIMEOUT_MS = 2000;
constexpr auto BULLET_TIMEOUT_MS = 1000;
constexpr auto FIRE_TIMEOUT_MS = 500;

Bot::Bot(int health, int numBullets, int numGrenades, Team& team, vec2f& location,
	vec2f& maxSpeed, vec2f& target, float boundingDiameter, BoardCells& board)
	: health(health), numBullets(numBullets), numGrenades(numGrenades), team(team), pathFinder(*this),
	CellMovingObject(location, maxSpeed, target, boundingDiameter, board), teamColor(DrawerColor::WHITE),
	lastBulletShot(chrono::high_resolution_clock::now()), lastGrenadeShot(chrono::high_resolution_clock::now()),
	lastFire(chrono::high_resolution_clock::now())
{

}

void Bot::updateEyeDireciton(vec2f& lookAt)
{
	lookingAt = (lookAt - location).normalize() / 20.0f;
}

void Bot::setTargetAndEyesDirection(BoardCells& board, Cell& nextLocation)
{
	vec2f targetLocation = getCellCenter(board, nextLocation);
	updateEyeDireciton(targetLocation);
	setTarget(board, nextLocation);
}

void Bot::roadToTargetAtTheSameRoom(Cell* target)
{
	map<Cell*, Cell*> pathToTarget = pathFinder.getRoomPath(target);
	Cell* mylocation = &getCellLocation();
	Cell& nextlocation = *pathToTarget[mylocation];
	setTargetAndEyesDirection(*board, nextlocation);
}



bool Bot::canFire(chrono::high_resolution_clock::time_point& shot, double shotTimeoutMs)
{
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();

	chrono::duration<double, std::milli> timePassedShotMS = now - shot;
	chrono::duration<double, std::milli> timePassedFireMS = now - lastFire;
	if (timePassedShotMS.count() < shotTimeoutMs || timePassedFireMS.count() < FIRE_TIMEOUT_MS) {
		return false;
	}

	shot = now;
	lastFire = now;
	return true;
}

void Bot::shootBullet(Cell& t)
{
	float boundingBulletDiameter = 0.25;
	int damage = 30;

	if (!canFire(lastBulletShot, BULLET_TIMEOUT_MS)) {
		return;
	}

	numBullets--;

	vec2f src = location;
	vec2f tgt = getCellCenter(*board, t);
	vec2f dir = tgt - src;
	calcEdgeTarget(src, dir, tgt, boundingBulletDiameter / 2.f);
	
	vec2f maxSpeed(MAX_BULLET_SPEED, MAX_BULLET_SPEED);
	
	Projectile* p = new Bullet(src, maxSpeed, tgt, boundingBulletDiameter, damage, &team);
	team.registerProjectile(*p);
	updateEyeDireciton(tgt);
}

void Bot::calcEdgeTarget(vec2f& location, vec2f& direction, vec2f& myTarget, float boundingRadius)
{
	Room* room = static_cast<Room*>(board);
	vec2f locationRelate2Room = location - room->getXYOffset();

	// Note: the calculation is based on "room' system' axis"
	vec2f collisionResult = CollisionLogic::calcCollision(room, locationRelate2Room, direction, boundingRadius);
	myTarget.set(room->getXYOffset().x + collisionResult.x, room->getXYOffset().y + collisionResult.y);
}

void Bot::throwGrenade(Cell& t)
{
	float boundingGrenadeDiameter = 0.25f;
	int damage = 1;

	if (!canFire(lastGrenadeShot, GRENADE_TIMEOUT_MS)) {
		return;
	}

	numGrenades--;

	vec2f src = getCellCenter(*board, getCellLocation());
	vec2f tgt = getCellCenter(*board, t);
	/*vec2f dir = tgt - src;
	calcEdgeTarget(src, dir, tgt, boundingGrenadeDiameter / 2.f);*/

	vec2f speed(MAX_GRENADE_SPEED, MAX_GRENADE_SPEED);
	
	/* grenade timeout is 1 - 3 seconds*/
	int exploasionTimeoutMS = (rand() % 2000) + 1000;
	int numFragments = 30;

	Projectile* p = new Grenade(src, speed, tgt, boundingGrenadeDiameter, damage, exploasionTimeoutMS, numFragments, &team);
	team.registerProjectile(*p);
	updateEyeDireciton(tgt);
}


void Bot::fight(Cell* target)
{
	Room* room = dynamic_cast<Room*>(board);
	if (!room) {
		cout << __func__ << "room is a nullptr" << endl;
		return;
	}
	Cell& mylocation = getCellLocation();
	if (CollisionLogic::isLineOfSight(*room, mylocation,*target, boundingDiameter)) {
		if (numBullets > 0) {
			shootBullet(*target);
		}
		
		if (numGrenades > 0) {
			throwGrenade(*target);
		}
	}
	else {
		roadToTargetAtTheSameRoom(target);
	}
}

void Bot::roaming(stack<GamePoint>& roamingPath)
{
	GamePoint destLocation = roamingPath.top();
	BoardCells* targetBoard = destLocation.board;
	Cell* targetCell = destLocation.cell;
	Cell& nextlocation = *targetCell;
	if (targetBoard) {
		setTargetAndEyesDirection(*targetBoard, nextlocation);
	}
	else {
		setTargetAndEyesDirection(*board, nextlocation);
	}
	roamingPath.pop();
}

void Bot::roadToTeammate(GamePoint& destLocation)
{
	BoardCells* targetBoard = destLocation.board;
	Cell* targetCell = destLocation.cell;

	if (isTargetAtTheSameRoom(targetBoard)) {
		roadToTargetAtTheSameRoom(targetCell);
	}
	else {
		setTargetAndEyesDirection(*targetBoard,*targetCell);
	}
}

void Bot::roadToConsumable(stack<GamePoint>& pathToConsumable)
{
	if (pathToConsumable.empty()) {
		SupportBot* supportBot = team.isSupportBotAlive(*this);
		if (supportBot == nullptr) {
			findEnemy();
		}
		else {
			stack<GamePoint> pathToTeammate = pathFinder.searchTeammate(*supportBot);
			if (pathToTeammate.empty()) {
				cout << __func__ << " Path to teammate is empty although teammate is alive" << endl;
			}
			GamePoint destLocation = pathToTeammate.top();
			float distanceFromSupportBot = manhattan_distance(destLocation.cell, &getCellLocation());
			/* if bot is close to the support bot and support bot cant give health go find enemies instead */
			if (distanceFromSupportBot < MAX_DIST_FOR_SUPPORTING && !supportBot->getCanGiveHealth()) {
				findEnemy();
				return;
			}

			roadToTeammate(destLocation);
			pathToTeammate.pop();
		}
	}
	else {
		GamePoint consumableLocation = pathToConsumable.top();
		pathToConsumable.pop();

		BoardCells* tb = consumableLocation.board;
		Cell* tc = consumableLocation.cell;

		if (isTargetAtTheSameRoom(tb)) {
			roadToTargetAtTheSameRoom(tc);
		}
		else {
			setTargetAndEyesDirection(*tb, *tc);
		}
	}
}

void Bot::roadToEnemy(stack<GamePoint>& pathToEnemy)
{
	GamePoint destLocation = pathToEnemy.top();
	BoardCells* targetBoard = destLocation.board;
	Cell* targetCell = destLocation.cell;
	pathToEnemy.pop();

	if (isTargetAtTheSameRoom(targetBoard)) {
		if (!targetBoard) {
			fight(targetCell);
		}
		else {
			roadToTargetAtTheSameRoom(targetCell);
		}
	}
	else {
		setTargetAndEyesDirection(*targetBoard ,*targetCell);
	}
}

void Bot::findHealth() 
{
	stack<GamePoint> pathToHealthBox = pathFinder.searchClosetHealth();
	roadToConsumable(pathToHealthBox);
}


void Bot::findAmmo()
{
	stack<GamePoint> pathToAmmoBox = pathFinder.searchClosetAmmo();
	roadToConsumable(pathToAmmoBox);
}

void Bot::findEnemy()
{
	stack<GamePoint> pathToEnemy = pathFinder.searchClosetEnemy();
	if (pathToEnemy.empty()) {
		stack<GamePoint> roamingPath = pathFinder.roam();
		roaming(roamingPath);
		return;
	}
	roadToEnemy(pathToEnemy);
}

void Bot::update()
{
	this->move();
	this->updateLastLocation();

	if (!this->isAtTarget()) {
		return;
	}

	if (health < HEALTH_THRESHOLD) {
		findHealth();
	}
	else if (numBullets < AMMO_THRESHOLD_BULLET) {
		findAmmo();
	}
	else {
		findEnemy();
	}
}

void Bot::draw()
{
	float x = location.x;
	float y = location.y;

	Drawer::filledCircle(x, y, boundingDiameter, DrawerColor::BLACK);
	Drawer::filledCircle(x, y, boundingDiameter / 1.5f, teamColor);
	Drawer::filledCircle(x + lookingAt.x, y + lookingAt.y, boundingDiameter / 2.0f, DrawerColor::BLACK);

	float healthPrecents = static_cast<float>(health / static_cast<float>(MAX_HEALTH));
	float maxWidth = 1.5f;
	float w = healthPrecents * maxWidth;
	float h = 0.2f;
	Drawer::rect(x - (maxWidth/2.f), y - boundingDiameter, w, h, DrawerColor::RED);
}