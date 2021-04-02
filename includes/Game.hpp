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

	CellMat generateCellMat(int numRows, int numCols);
	std::vector<std::vector<vec2i>> generateRoomsOn2DGrid(vec2i& gridOffset, vec2i& maxWH);
	std::vector<std::vector<vec2i>> generateCorridorsOn2DGrid(std::vector<std::vector<vec2i>>& roomsWH, vec2i& gridOffset);
	void connectRoomCorridorLeftToRight(int i, int j, std::vector<vec2i>& corridorsLeftToRight, vec2i& gridOffset);
	void connectRoomCorridorTopToBottom(int i, int j, std::vector<vec2i>& corridorsLeftToRight, vec2i& gridOffset);
	void connectRoomsCorridors(std::vector<std::vector<vec2i>>& corrdirosOffsets, vec2i& gridOffset);
	void randomizeMap();

	void createTeam(Team& t, int roomRow);
	void addConsumbles();
	void addObstacles();
	void removeConsumablesOccupation();

	Game();
	static Game* game;
public:
	Game(const Game& other) = delete;
	void operator=(const Game& other) = delete;
	static Game* getInstance();
	static std::vector<Team>& getGameTeams() { return game->getTeams(); }
	static std::vector<Room>& getGameRooms() { return game->getRooms(); }
	static std::vector<Corridor>& getGameCorridors() { return game->getCorridors(); }
	void createTeams();
	Room* getRoom(vec2f& cellLocation);

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