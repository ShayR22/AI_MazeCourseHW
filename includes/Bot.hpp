#ifndef __BOT__
#define __BOT__

#include "CellMovingObject.hpp"
#include "Team.hpp"
#include "PathFinder.hpp"
#include "Drawer.hpp"

#define MAX_BULLETS 20
#define MAX_GRENADES 20
#define MAX_HEALTH 100
#define HEALTH_THRESHOLD 50
#define AMMO_THRESHOLD 10

class Bot : public CellMovingObject {
private:
	int health;
	int numBullets;
	int numGrenades;
	Team& team;
	PathFinder pathFinder;
	DrawerColor teamColor;

	/*pathfinder returns nullptr board if the target is at the same boardcell with the player*/
	inline bool isTargetAtTheSameRoom(BoardCells* targetBoard) { return !targetBoard || (board.getShootable() && targetBoard->getShootable());}
	void roadToTargetAtTheSameRoom(Cell* target);
	void roadToTeammate(GamePoint& destLocation);
	void roadToConsumable(std::stack<GamePoint>& pathToConsumable);
	void roadToEnemy(std::stack<GamePoint>& pathToEnemy);
	void roaming(std::stack<GamePoint>& roamingPath);
	void findHealth();
	void findAmmo();
	void findEnemy();
	void fight(Cell* target);

public:
	Bot(int health, int numBullets, int numGrenades, Team& team, vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board);

	void updateBot();

	inline int getHealth() { return health; }
	inline int getNumBullets() { return numBullets; }
	inline int getNumGrenades() { return numGrenades; }
	inline Team& getTeam() { return team; }
	inline DrawerColor getTeamColor() { return teamColor; }

	inline void setHealth(int health) { this->health = health; };
	inline void setNumBullets(int numBullets) { this->numBullets = numBullets; }
	inline void setNumGrenades(int numGrenades) { this->numGrenades = numGrenades; }
	inline void setTeam(Team& team) { this->team = team; }
	inline void setTeamColor(DrawerColor teamColor) { this->teamColor = teamColor; }


	inline bool isAmmoFull() { return (numBullets < MAX_BULLETS &&  + numGrenades < MAX_GRENADES); }
	inline bool isFullHealth() { return health < MAX_HEALTH; }

	virtual void draw();
};

#endif