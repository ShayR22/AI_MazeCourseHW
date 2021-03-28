#ifndef __BOT__
#define __BOT__

#include "CellMovingObject.hpp"
#include "Team.hpp"
#include "PathFinder.hpp"

#define MAX_AMMO 20
#define MAX_HEALTH 100

class Bot : public CellMovingObject {
private:
	int health;
	int numBullets;
	int numGrenades;
	int ammoNum;
	Team& team;
	PathFinder pathFinder;
public:
	Bot(int health, int getNumBullets, int getNumGrenades);

	void update();

	inline int getHealth() { return health; }
	inline int getNumBullets() { return numBullets; }
	inline int getNumGrenades() { return numGrenades; }
	inline int getNumAmmo() { return ammoNum; }
	inline Team& getTeam() { return team; }
	inline PathFinder& getPathFinder() { return pathFinder; }

	inline void setHealth(int health) { this->health = health; };
	inline void setNumBullets(int numBullets) { this->numBullets = numBullets; }
	inline void setNumGrenades(int numGrenades) { this->numGrenades = numGrenades; }
	inline void setNumAmmo(int numAmmo) { this->ammoNum = ammoNum; }
	inline void setTeam(Team& team) { this->team = team; }
	inline void setPathFinder(PathFinder& pathFinder) { this->pathFinder = pathFinder; }

	inline bool isAmmoFull() { return (numBullets + numGrenades) < MAX_AMMO; }
	inline bool isFullHealth() { return health < MAX_HEALTH; }
};

#endif