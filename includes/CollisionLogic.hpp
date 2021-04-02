#ifndef __COLLISIONLOGIC__
#define __COLLISIONLOGIC__

#include "Team.hpp"
#include "Room.hpp"
#include "Cell.hpp"
#include "vec2.h"
#include "Line.h"
#include <limits>
#include <math.h>
#include <vector>

class CollisionLogic {
private:
	std::vector<Team> teams;
public:
	CollisionLogic() {};
	CollisionLogic(std::vector<Team> teams);
	void handleCollisions();

	inline void setTeams() { this->teams = teams; }
	inline std::vector<Team>& getTeams() { return teams; }

	static vec2f getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points);
	static void getCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points, vec2f& locationResult, float& distanceResult);
	static std::vector<vec2f> extractShape(std::vector<Cell*>& cover);
	static bool isCollision(vec2f& p, vec2f& dir, std::vector<vec2f>& points);
	static bool isLineOfSight(Room& r, Cell& src, Cell& tgt);

	static vec2f calcCollision(Room* room, vec2f& location, vec2f& speed);


private:
	static void getCollisionTilLine(vec2f& point, vec2f& dir, Line2D& line, vec2f& locationResult, float& distanceResult);
	
};

#endif