#include "MazeGame.h"
#include "OpenGL.h"

using namespace std;

void MazeGame::initalizeMaze(int size)
{
	numCoins = size * size;
	maze = new Maze(size, size);
	maze->setColorHidden(true);

	start = maze->getStarts()[0];

	cell_mat& cells = maze->getCells();
	for (auto& cellsRow : cells)
		for (auto& cell : cellsRow)
			cell.setHasCoin(true);
}

void MazeGame::initalizePlayer()
{
	constexpr auto PLAYER_MOVEMENT_PER_FRAME = 1;

	float sx = start->getX() + 0.5f;
	float sy = start->getY() + 0.5f;
	float dx = 2 * 1.0f / OpenGL::width * PLAYER_MOVEMENT_PER_FRAME;
	float dy = 2 * 1.0f / OpenGL::height * PLAYER_MOVEMENT_PER_FRAME;
	float tx = maze->getTarget().getX() + 0.5f;
	float ty = maze->getTarget().getY() + 0.5f;
	player = new Player(maze->getCells(), sx, sy, dx, dy, tx, ty, numCoins);

}

void MazeGame::initalizeEnemies()
{
	constexpr auto NUM_ENEMIES = 3;
	constexpr auto ENEMY_MOVEMENT_PER_FRAME = 1.0f;

	int size = (int)maze->getCells().size();

	int xys[3][2] = {
		{0       , 0},
		{size - 1, 0},
		{size - 1, size - 1},
	};

	for (int i = 0; i < NUM_ENEMIES; i++) {
		float sx = xys[i][0] + 0.5f;
		float sy = xys[i][1] + 0.5f;
		float dx = 2.0f * 1.0f / OpenGL::width * ENEMY_MOVEMENT_PER_FRAME;
		float dy = 2.0f * 1.0f / OpenGL::height * ENEMY_MOVEMENT_PER_FRAME;
		enemies.push_back(Enemy(maze->getCells(), sx, sy, dx, dy, -1, -1));
	}
}

void MazeGame::reCalculatePaths()
{
	Cell& pLocation = player->getCellLocation();
	enemiesPaths.clear();
	for (auto& e : enemies) {
		Cell& eLocation = e.getCellLocation();
		enemyBrain->setStartTarget(eLocation, pLocation);
		enemyBrain->solve();
		enemiesPaths.push_back(enemyBrain->getNextInPath());
	}

}

void MazeGame::initalizeEnemiesBrain()
{
	enemyBrain = new MazeSolverAStar(*maze);
	reCalculatePaths();

	for (int i = 0; i < (int)enemies.size(); i++) {
		Cell* eLocation = &enemies[i].getCellLocation();
		Cell* nextTargetInPath = enemiesPaths[i][eLocation];
		if (nextTargetInPath == nullptr)
			throw "enemy location isn't in path to target";

		enemies[i].setTarget(*nextTargetInPath);
	}
}

MazeGame::MazeGame(int size)
{
	solved = false;
	initalizeMaze(size);
	initalizePlayer();
	initalizeEnemies();
	initalizeEnemiesBrain();
}

MazeGame::~MazeGame()
{
	delete maze;
	delete player;
	delete enemyBrain;
}

void MazeGame::draw()
{
	maze->draw();
	player->draw();
	for (auto& e : enemies)
		e.draw();
}

void MazeGame::updateTargetLocation(MazeMovingObj &o, map<Cell*, Cell*> nextInPath)
{
	if (!o.isAtTarget())
		return;

	Cell* current = &o.getCellLocation();
	Cell* target = nextInPath[current];
	if (target) {
		o.setTarget(*target);
	}
}

void MazeGame::updateEnemies()
{
	for (int i = 0; i < (int)enemies.size(); i++) {
		enemies[i].move();
		updateTargetLocation(enemies[i], enemiesPaths[i]);
	}
}

bool MazeGame::isEnemyGotPlayer()
{
	/* TODO add smart solution for dynamic radius size */
	float radiusesSize = 2.0f * OpenGL::circleR / 1.2f;


	float px = player->getX();
	float py = player->getY();

	for (auto& e : enemies) {
		float ex = e.getX();
		float ey = e.getY();

		float xDiff = px - ex;
		float yDiff = py - ey;
		float dist = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
		if (dist < radiusesSize)
			return true;
	}

	return false;
}


void MazeGame::solveIteration()
{
	if (solved)
		return;

	updateEnemies();
	
	/*
	Enemies:
		
		move
		updateTargetLocation
		if 5 seconds passed recalculate paths
	
	Player:
		TODO add it

	GameLogic:
		test if enemy got player

	*/
	

	//player->move();
	if (numCoins == 0) {
		solved = true;
		cout << "numCoins is zero " << endl;
	}

	if (isEnemyGotPlayer()) {
		solved = true;
		cout << "player got hit by enemy" << endl;
	}
	
}