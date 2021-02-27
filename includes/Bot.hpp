#ifndef __BOT__
#define __BOT__

#include "CellMovingObject.hpp"
#include "Team.hpp"
#include "PathFinder.hpp"

class Bot : public CellMovingObject {
private:
	int health;
	int numBullets;
	int numGrenades;
	int numAmmo;
	Team& team;
	PathFinder pathFinder;
public:
	void update();

	inline int getHealth() { return health; }
	inline int getNumBullets() { return numBullets; }
	inline int getNumGrenades() { return numGrenades; }
	inline int getNumAmmo() { return numAmmo; }
	inline Team& getTeam() { return team; }
	inline PathFinder& getPathFinder() { return pathFinder; }

	inline void setHealth(int health) { this->health = health; };
	inline void setNumBullets(int numBullets) { this->numBullets = numBullets; }
	inline void setNumGrenades(int numGrenades) { this->numGrenades = numGrenades; }
	inline void setNumAmmo(int numAmmo) { this->numAmmo = numAmmo; }
	inline void setTeam(Team& team) { this->team = team; }
	inline void setPathFinder(PathFinder& pathFinder) { this->pathFinder = pathFinder; }
};

#endif