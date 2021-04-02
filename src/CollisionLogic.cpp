#include "CollisionLogic.hpp"
#include "Game.hpp"

#include "Cell.hpp"
#include "Bot.hpp"
#include <iostream>

using namespace std;

CollisionLogic::CollisionLogic() : teams(Game::getGameTeams()), rooms(Game::getGameRooms()), corridors(Game::getGameCorridors())
{

}

void CollisionLogic::handleCollisions()
{
	/*
	* handle collision between:
	* - bot vs consumables
	* - bot vs projectiles
	* - projectile vs obstacle
	*/

	handleCollisionBetweenBots2Consumables();
	handleCollisionBetweenBots2Projectiles();
}

void CollisionLogic::handleCollisionBetweenBots2Consumables()
{
	for (auto& team : teams) {
		for (auto& bot : team.getBots()) {
			handleCollisionBetweensingleBot2Consumables(bot);
		}
	}
	
}

void CollisionLogic::handleCollisionBetweensingleBot2Consumables(Bot* bot)
{
	BoardCells* board = &bot->getBoardCells();
	if (!board->getShootable())
		return;

	Room* room = static_cast<Room*>(board);

	Cell* src = &bot->getCellLocation();

	for (auto& consumable : room->getConsumables(ConsumableType::AMMO)) {
		Cell* dst = consumable->getLocation();
		if (dst == src) {
			consumable->consume(*bot);
			return;
		}
	}
	
	for (auto& consumable : room->getConsumables(ConsumableType::HEALTH)) {
		Cell* dst = consumable->getLocation();
		if (dst == src) {
			consumable->consume(*bot);
			return;
		}
	}
}

void CollisionLogic::handleCollisionBetweenBots2Projectiles()
{
	vector<Bot*> bots;
	for (auto& room : rooms) {
		for (auto& enemyTeam : teams) {

			for (auto& team : teams) {
				// check only in teams which are NOT the same team of enemy projectiles
				if (&team == &enemyTeam) {
					continue;
				}	
				// TODO:

				bots = getBotsInsideRoom(room, team);
				handleCollisionBetweenTeam2EnemyProjectilesInRoom(room, bots, enemyTeam);
			}
		}
	}
}

std::vector<Bot*> CollisionLogic::getBotsInsideRoom(Room& room, Team& team)
{
	vector<Bot*> bots;
	for (auto& bot : team.getBots()) {
		if (&bot->getBoardCells() == &room)
			bots.push_back(bot);
	}
	return bots;
}

vector<Projectile*> CollisionLogic::extractProjectileInRoom(Room &room, Team &team)
{
	vec2f projectileLocation;
	vector<vec2f> points;
	vector<Projectile*> projectilesResult;

	room.getShape(points);
	float xmin = points[0].x, xmax = points[2].x, ymin = points[0].y, ymax = points[2].y;
	for (auto& projectile : team.getProjectiles()) {
		projectileLocation = projectile->getLocation();
		if (projectileLocation.x >= xmin && projectileLocation.x <= xmax
			&& projectileLocation.y >= ymin && projectileLocation.y <= ymax)
			projectilesResult.push_back(projectile);
	}

	return projectilesResult;
}


// ASSUNPTION: Enemy projectiles ate NOT relate to the team!
void CollisionLogic::handleCollisionBetweenTeam2EnemyProjectilesInRoom(Room& room, vector<Bot*> bots, Team& enemyTeam)
{
	vector<Projectile*> enemyProjectiles;

	Projectile* projectile;

	for (auto& bot : bots) {
		// each iteration we want to get the updated enemy projectiles
		enemyProjectiles = extractProjectileInRoom(room, enemyTeam);
		for (unsigned int pos = 0; pos < enemyProjectiles.size(); pos++) {
			projectile = enemyProjectiles[pos];
			if (isBotClose2Projectile(bot, projectile)) {
				bot->decreaseHealth(static_cast<int>(projectile->calculatePower()));
				enemyTeam.unregisterProjectile(pos);


			}
		}


		// to be sure it was cleaned
		enemyProjectiles.clear();
	}
}

bool CollisionLogic::isBotClose2Projectile(Bot* bot, Projectile* projectile)
{
	vec2f v = projectile->getLocation();
	return bot->getLocation().dist(v) <= BOT_RADIUS + projectile->getBoundingRadius();
}

vec2f CollisionLogic::getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points) {
	Line2D line;
	float dist = FLT_MAX;
	float curDist;
	vec2f curLoc, loc;

	size_t s = points.size();
	for (size_t i = 0; i < s; ++i) {
		line.set(points[i % s], points[(i + 1) % s]);
		getCollisionTilLine(p, dir, line, curLoc, curDist);
		if (curDist < dist) {
			dist = curDist;
			loc.set(curLoc.x, curLoc.y);
		}
	}

	return loc;
}

void CollisionLogic::getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points, vec2f& locationResult, float& distanceResult) {
	Line2D line;
	float dist = FLT_MAX;
	float curDist;
	vec2f curLoc, loc;

	size_t s = points.size();
	for (size_t i = 0; i < s; ++i) {
		line.set(points[i % s], points[(i + 1) % s]);
		getCollisionTilLine(p, dir, line, curLoc, curDist);
		if (curDist < dist) {
			dist = curDist;
			loc.set(curLoc.x, curLoc.y);
		}
	}

	locationResult.set(loc.x, loc.y);
	distanceResult = dist;
}

bool CollisionLogic::isCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points) {
	Line2D line;
	float curDist;
	vec2f curLoc, loc;

	size_t s = points.size();
	for (size_t i = 0; i < s; ++i) {
		line.set(points[i % s], points[(i + 1) % s]);
		getCollisionTilLine(p, dir, line, curLoc, curDist);
		if (curDist < FLT_MAX) {
			return true;
		}
	}

	return false;
}

void CollisionLogic::getCollisionTilLine(vec2f& point, vec2f& dir, Line2D& line, vec2f& locationResult, float& distanceResult) {
	vec2f targetPoint = point + dir;
	Line2D vecLine(point, targetPoint);

	if (vecLine.isParallelTo(line)) {
		distanceResult = FLT_MAX;
		return;
	}

	vec2f intersectPoint;
	Line2D::getIntersectPoint(line, vecLine, intersectPoint);

	vec2f dirToLine = intersectPoint - point;
	if (!dir.hasSameDirection(dirToLine)) {
		distanceResult = FLT_MAX;
		return;
	}

	if (!line.isInLineLimits(intersectPoint)) {
		distanceResult = FLT_MAX;
		return;
	}

	locationResult.set(intersectPoint.x, intersectPoint.y);
	distanceResult = point.dist(intersectPoint);
}

vector<vec2f> CollisionLogic::extractShape(vector<Cell*>& cover)
{
	float minX = 1000;
	float minY = 1000;
	float maxX = -1;
	float maxY = -1;

	for (auto& c : cover) {
		if (minX > static_cast<float>(c->getX()))
			minX = static_cast<float>(c->getX());
		if (maxX < static_cast<float>(c->getX()))
			maxX = static_cast<float>(c->getX());
		if (minY > static_cast<float>(c->getY()))
			minY = static_cast<float>(c->getY());
		if (maxY < static_cast<float>(c->getY()))
			maxY = static_cast<float>(c->getY());
	}

	// set the max-limit as the start of the next highest cell
	// for example: wall which conatinas Cells (2,2), (2,3) will have the points: min=(2,2), max=(3, 4) 
	maxX += 1;
	maxY += 1;

	vector<vec2f> shape = { {minX, maxY}, {maxX, maxY}, {maxX, minY}, {minX, minY} };
	return shape;
}


bool CollisionLogic::isLineOfSight(Room& r, Cell& src, Cell& tgt)
{
	vec2f srcP(static_cast<float>(src.getX()), static_cast<float>(src.getY()));
	vec2f dir(static_cast<float>(tgt.getX() - src.getX()),
		static_cast<float>(tgt.getY() - src.getY()));

	vector<vector<vec2f>> shapes;
	for (auto& o : r.getObstacles()) {
		vector<Cell*> cover = o->getCover();
		vector<vec2f> shape = CollisionLogic::extractShape(cover);
		shapes.push_back(shape);
	}

	for (auto& shape : shapes) {
		if (CollisionLogic::isCollision(srcP, dir, shape))
			return false;
	}

	return true;
}

vec2f CollisionLogic::calcCollision(Room *room, vec2f& location, vec2f& speed)
{
	float curMinDist = FLT_MAX;
	vec2f curMinLoc;
	vector<vec2f> roomShape;
	vec2f locationResult;
	float distanceResult;
	

	// understand if there is collision with the room itself:
	float w = room->getWidth();
	float h = room->getHeight();
	roomShape.push_back(vec2f(0, 0));
	roomShape.push_back(vec2f(w, 0));
	roomShape.push_back(vec2f(w, h));
	roomShape.push_back(vec2f(0, h));
	
	CollisionLogic::getCollision(location, speed, roomShape, curMinLoc, curMinDist);

	if (curMinDist >= FLT_MAX) {
		// if there is no collision with the room, it's a problem
		cout << "There is a problem in: " << __func__ << ", the mooving object is out from room!" << endl;
		return vec2f(-1, -1);
	}

	distanceResult = curMinDist;
	locationResult = curMinLoc;

	vector<vector<vec2f>> shapes;
	for (auto& o : room->getObstacles()) {
		vector<Cell*> cover = o->getCover();
		vector<vec2f> obstcleShape = CollisionLogic::extractShape(cover);
		
		CollisionLogic::getCollision(location, speed, obstcleShape, curMinLoc, curMinDist);

		if (curMinDist < distanceResult) {
			distanceResult = curMinDist;
			locationResult = curMinLoc;
		}
	}

	return locationResult;
}