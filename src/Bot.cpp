#include "Bot.hpp"
#include "CollisionLogic.hpp"
#include "Drawer.hpp"
#include "Cell.hpp"
#include "GamePoint.hpp"
#include "Bullet.hpp"
#include "Grenade.hpp"
#include <iostream>
#include <stack>
#include <map>

using namespace std;

constexpr auto GRENADE_TIMEOUT_MS = 2000;
constexpr auto BULLET_TIMEOUT_MS = 1000;
constexpr auto FIRE_TIMEOUT_MS = 100;

Bot::Bot(int health, int numBullets, int numGrenades, Team& team, vec2f& location,
	vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board)
	: health(health), numBullets(numBullets), numGrenades(numGrenades), team(team), pathFinder(*this),
	CellMovingObject(location, maxSpeed, target, boundingRadius, board), teamColor(DrawerColor::WHITE),
	lastBulletShot(chrono::high_resolution_clock::now()), lastGrenadeShot(chrono::high_resolution_clock::now()),
	lastFire(chrono::high_resolution_clock::now())
{

}

void Bot::roadToTargetAtTheSameRoom(Cell* target)
{
	map<Cell*, Cell*> pathToTarget = pathFinder.getRoomPath(target);
	Cell* mylocation = &getCellLocation();
	Cell& nextlocation = *pathToTarget[mylocation];
	setTarget(*board,nextlocation);
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
	if (!canFire(lastBulletShot, BULLET_TIMEOUT_MS)) {
		return;
	}

	numBullets--;

	vec2f src = location;
	vec2f tgt = getCellCenter(*board, t);
	vec2f maxSpeed(MAX_BULLET_SPEED, MAX_BULLET_SPEED);
	float boundingRadius = 0.25;
	int damage = 15;
	
	Projectile* p = new Bullet(src, maxSpeed, tgt, boundingRadius, damage, &team);
	team.registerProjectile(*p);

}

void Bot::throwGrenade(Cell& t)
{
	if (!canFire(lastGrenadeShot, GRENADE_TIMEOUT_MS)) {
		return;
	}

	numGrenades--;

	vec2f src = getCellCenter(*board, getCellLocation());
	vec2f tgt = getCellCenter(*board, t);
	vec2f speed(MAX_GRENADE_SPEED, MAX_GRENADE_SPEED);
	float boundingRadius = 0.4f;
	int damage = 1;
	int exploasionTimeoutMS = 400;
	int numFragments = 5;

	Projectile* p = new Grenade(src, speed, tgt, boundingRadius, damage, exploasionTimeoutMS, numFragments, &team);
	team.registerProjectile(*p);
}


void Bot::fight(Cell* target)
{
	Room* room = dynamic_cast<Room*>(board);
	if (!room) {
		cout << __func__ << "room is a nullptr" << endl;
		return;
	}
	Cell& mylocation = getCellLocation();
	if (CollisionLogic::isLineOfSight(*room, mylocation,*target)) {
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
		setTarget(*targetBoard, nextlocation);
	}
	else {
		setTarget(*board, nextlocation);
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
		setTarget(*targetBoard,*targetCell);
	}
}

void Bot::roadToConsumable(stack<GamePoint>& pathToConsumable)
{
	if (pathToConsumable.empty()) {
		Bot* supportBot = team.isSupportBotAlive();
		if (supportBot == nullptr) {
			findEnemy();
		}
		else {
			stack<GamePoint> pathToTeammate = pathFinder.searchTeammate(*supportBot);
			if (pathToTeammate.empty()) {
				cout << __func__ << " Path to teammate is empty although teammate is alive" << endl;
			}
			GamePoint destLocation = pathToTeammate.top();
			roadToTeammate(destLocation);
			pathToTeammate.pop();
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
		setTarget(*targetBoard ,*targetCell);
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
	else if (numBullets + numGrenades <= AMMO_THRESHOLD) {
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

	Drawer::filledCircle(x, y, boundingRadius, DrawerColor::BLACK);
	Drawer::filledCircle(x, y, boundingRadius / 1.5f, teamColor);
}