#ifndef __PATHFINDER__
#define __PATHFINDER__

class PathFinder {
	Cell* searchEnemy();
	Cell* searchTeammate();
	Cell* searchAmmo();
	Cell* searchHealth();
	void roam();
	void setSource(Bot& bot);
	void setTarget();
};

#endif