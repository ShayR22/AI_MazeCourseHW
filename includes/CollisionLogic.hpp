#ifndef __COLLISIONLOGIC__
#define __COLLISIONLOGIC__

#include "Team.hpp"
#include <vector>

class CollisionLogic {
private:
	std::vector<Team> teams;
public:
	CollisionLogic(std::vector<Team> teams);
	void handleCollisions();

	inline void setTeams() { this->teams = teams; }
	inline std::vector<Team>& getTeams() { return teams; }
};

#endif