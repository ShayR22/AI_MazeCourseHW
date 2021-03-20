#ifndef __COLLISIONLOGIC__
#define __COLLISIONLOGIC__

#include "Team.hpp"
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

private:
	double getDistTilCollidLine(vec2f& point, vec2f& dir, Line2D& line);
	double getCollisionDistance(vec2f& p, vec2f& dir, std::vector<vec2f>& points);
};

#endif