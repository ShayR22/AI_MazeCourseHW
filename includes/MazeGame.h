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
#include "BFSSolver.h"

class MazeGame : public Drawable, public Solveable {
private:
	int numCoins;

	Maze *maze;
	Cell* start;
	Player *player;

	std::vector<Enemy> enemies;
	std::map<Enemy*, std::map<Cell*, Cell*>> enemiesPaths;
	std::map<Cell*, std::vector<std::vector<Cell*>>> enemiesPathToPlayer; //yoni
	MazeSolverAStar* enemyBrain;
	BFSSolver* playerEscapeBrain;

	void updateTargetLocation(MazeMovingObj& o, std::map<Cell*, Cell*> nextInPath);
	void updateEnemies();

	std::map<Cell*, Cell*> findCoinPath();
	std::vector<Cell*> calculatePossibleEscapePaths(Cell* c);
	std::map<Cell*, Cell*> findEscapePath(std::vector<MazeMovingObj*> enemies);
	std::vector<MazeMovingObj*> areEnemiesInRange();
	void updatePlayer();

	void initalizeMaze(int size);
	void initalizePlayer();
	void initalizePlayerEscapeBrain();
	void initalizeEnemies();
	void initalizeEnemiesBrain();
	void reCalculatePaths(std::vector<Enemy*> requireBrainUpdate);
	void enemyBrainTick();
	bool isEnemyGotPlayer();

public:
	MazeGame(int size);
	~MazeGame();
	virtual void draw() override;
	virtual void solveIteration() override;
};


#endif // !__MAZE_GAME__
