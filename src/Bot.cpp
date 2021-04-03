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

Bot::Bot(int health, int numBullets, int numGrenades, Team& team, vec2f& location,
	vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board)
	: health(health), numBullets(numBullets), numGrenades(numGrenades), team(team), pathFinder(*this),
	CellMovingObject(location, maxSpeed, target, boundingRadius, board), teamColor(DrawerColor::WHITE)
{

}

void Bot::roadToTargetAtTheSameRoom(Cell* target)
{
	map<Cell*, Cell*> pathToTarget = pathFinder.getRoomPath(target);
	Cell* mylocation = &getCellLocation();
	Cell& nextlocation = *pathToTarget[mylocation];
	setTarget(board,nextlocation);
}


void Bot::shootBullet(Cell& t)
{
	static chrono::high_resolution_clock::time_point lastShot = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();

	chrono::duration<double, std::milli> timePassedMS = now - lastShot;
	if (timePassedMS.count() < 1000) {
		return;
	}
	lastShot = now;

	vec2f src = getCellCenter(board, getCellLocation());
	vec2f tgt = getCellCenter(board, t);
	vec2f dir = tgt - src;
	dir.normalize();
	vec2f speed = dir * MAX_BULLET_SPEED;
	float boundingRadius = 0.75;
	int damage = 10;
	
	Projectile* p = new Bullet(src, tgt, speed, boundingRadius, damage, &team);
	team.registerProjectile(*p);
}

void Bot::throwGrenade(Cell& t)
{
	static chrono::high_resolution_clock::time_point lastShot = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();

	chrono::duration<double, std::milli> timePassedMS = now - lastShot;
	if (timePassedMS.count() < 1000) {
		return;
	}
	lastShot = now;

	vec2f src = getCellCenter(board, getCellLocation());
	vec2f tgt = getCellCenter(board, t);
	vec2f dir = tgt - src;
	dir.normalize();
	vec2f speed = dir * MAX_GRENADE_SPEED;
	float boundingRadius = 0.75;
	int damage = 1;
	int exploasionTimeoutMS = 2 * 1000;
	int numFragments = 5;

	Projectile* p = new Grenade(src, speed, tgt, boundingRadius, damage, exploasionTimeoutMS, numFragments, &team);
	team.registerProjectile(*p);
}


void Bot::fight(Cell* target)
{
	Room* room = dynamic_cast<Room*>(&board);
	if (!room) {
		cout << __func__ << "room is a nullptr" << endl;
		return;
	}
	Cell& mylocation = getCellLocation();
	if (CollisionLogic::isLineOfSight(*room, mylocation,*target)) {
		if (numBullets > 0) {
			shootBullet(*target);
		}
		else if (numGrenades > 0 && numBullets < numGrenades) {
			throwGrenade(*target);
		}
		else {
			cout << "Out of Ammo" << endl;
		}
	}
	roadToTargetAtTheSameRoom(target);
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
		setTarget(board, nextlocation);
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
		if (targetBoard) {
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
	}
	roadToEnemy(pathToEnemy);

}

void Bot::updateBot()
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
	vec2f xy = getCellCenter(board, getCellLocation());
	float x = xy.x;
	float y = xy.y;

	Drawer::filledCircle(x, y, boundingRadius, DrawerColor::BLACK);
	Drawer::filledCircle(x, y, boundingRadius / 1.5f, teamColor);
}