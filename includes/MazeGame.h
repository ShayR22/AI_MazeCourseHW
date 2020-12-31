#ifndef __MAZE_GAME__
#define __MAZE_GAME__

#include <map>
#include <vector>
#include <chrono>
#include "Drawable.h"
#include "Solvable.h"
#include "Maze.h"
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "MazeSolverAStar.h"

class MazeGame : public Drawable, public Solveable {
private:
	int numCoins;

	Maze *maze;
	Cell* start;
	Player *player;

	std::vector<Enemy> enemies;
	std::vector<std::map<Cell*, Cell*>> enemiesPaths;
	MazeSolverAStar* enemyBrain;

	std::chrono::high_resolution_clock::time_point last_tick;

	void updateTargetLocation(MazeMovingObj& o, std::map<Cell*, Cell*> nextInPath);
	void updateEnemies();

	void initalizeMaze(int size);
	void initalizePlayer();
	void initalizeEnemies();
	void initalizeEnemiesBrain();
	void reCalculatePaths();
	void enemyBrainTick();
	bool isEnemyGotPlayer();

public:
	MazeGame(int size);
	~MazeGame();
	virtual void draw() override;
	virtual void solveIteration() override;
};


#endif // !__MAZE_GAME__
