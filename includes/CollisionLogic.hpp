#ifndef __COLLISIONLOGIC__
#define __COLLISIONLOGIC__

#include "Team.hpp"
#include "Room.hpp"
#include "Cell.hpp"
#include "Projectile.hpp"
#include "vec2.h"
#include "Line.h"
#include <limits>
#include <math.h>
#include <vector>
#include <thread>

class CollisionLogic {
private:
	std::vector<std::thread> projectilesCollisionThreads;
	std::vector<Team*>& teams;
	std::vector<Room*>& rooms;
	std::vector<Corridor*>& corridors;

	void handleCollisionProjectileBetweenTeamsThread(Team* team, Team* enemyTeam);
public:
	// CollisionLogic() {};
	CollisionLogic();
	CollisionLogic(const CollisionLogic& collisionLogic);
	void operator=(const CollisionLogic& collisionLogic);
	void handleCollisions();

	inline void setTeams() { this->teams = teams; }
	inline std::vector<Team*>& getTeams() { return teams; }

	static vec2f getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points);
	static void getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points, vec2f& locationResult, float& distanceResult);
	static std::vector<vec2f> extractShape(std::vector<Cell*>& cover, float diameter = 0);
	static bool isCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points);
	static bool isLineOfSight(Room& r, Cell& src, Cell& tgt, float obstacleSafeDiameter);

	static vec2f calcCollision(Room* room, vec2f& location, vec2f& speed, float radius);


private:
	static void getCollisionTilLine(vec2f& point, vec2f& dir, Line2D& line, vec2f& locationResult, float& distanceResult);

	void handleCollisionBetweenBots2Consumables();
	void handleCollisionBetweensingleBot2Consumables(Bot* bot);

	void handleCollisionBetweenBots2Projectiles();
	std::vector<Projectile*> extractProjectileInRoom(Room& room, Team& team);

	void handleCollisionBetweenTeam2EnemyProjectilesInRoom(Room& room, std::vector<Bot*> bots, Team& enemyTeam);
	bool isBotClose2Projectile(Bot* bot, Projectile* projectile);
	std::vector<Bot*> getBotsInsideRoom(Room& room, Team& team);
};

#endif