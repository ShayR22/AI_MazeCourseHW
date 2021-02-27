#ifndef __GAME__
#define __GAME__

#include "CollisionLogic.hpp"
#include "Team.hpp"
#include "Room.hpp"
#include "Corridor.hpp"
#include <vector>

class Game {
private:
	CollisionLogic collisionLogic;
	std::vector<Team> teams;
	std::vector<Room> rooms;
	std::vector<Corridor> corridors;
public:
	void draw();
	void start();
	
	inline void setCollisionLogic(CollisionLogic& collisionLogic) { this->collisionLogic = collisionLogic;}
	inline CollisionLogic& getCollisionLogic() { return collisionLogic; }

	inline void setTeams() { this->teams = teams; }
	inline std::vector<Team>& getTeams() { return teams; }

	inline void setRooms() { this->rooms = rooms; }
	inline std::vector<Room>& getRooms() { return rooms; }

	inline void setCorridors() { this->corridors = corridors; }
	inline std::vector<Corridor>& getCorridors() { return corridors; }
};

#endif