#include "MazeGame.h"
#include "OpenGL.h"
#include "heruistics.h"
#include <map>

using namespace std;
using namespace std::chrono;

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
	constexpr auto PLAYER_MOVEMENT_PER_FRAME = 1.5f;

	float sx = start->getX() + 0.5f;
	float sy = start->getY() + 0.5f;
	float dx = 2 * 1.0f / OpenGL::width * PLAYER_MOVEMENT_PER_FRAME;
	float dy = 2 * 1.0f / OpenGL::height * PLAYER_MOVEMENT_PER_FRAME;
	//float tx = maze->getTarget().getX() + 0.5f;
	//float ty = maze->getTarget().getY() + 0.5f;
	player = new Player(maze->getCells(), sx, sy, dx, dy, sx, sy, numCoins);

}

void MazeGame::initalizePlayerEscapeBrain()
{

	playerEscapeBrain = new BFSSolver(*maze);

}

void MazeGame::initalizeEnemies()
{
	constexpr auto NUM_ENEMIES = 3;
	constexpr auto ENEMY_MOVEMENT_PER_FRAME = 0.75f;

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

void MazeGame::reCalculatePaths(vector<Enemy*> requireBrainUpdate)
{

	Cell& pLocation = player->getCellLocation();

	// vector enemies
	// map<Enemy*, value> paths
	for (auto& e : requireBrainUpdate) {
		
		Cell& eLocation = e->getCellLocation();
		enemyBrain->setStartTarget(eLocation, pLocation);
		enemyBrain->solve();
		enemiesPaths[e] = enemyBrain->getNextInPath();
	}
}

void MazeGame::initalizeEnemiesBrain()
{
	enemyBrain = new MazeSolverAStar(*maze);

	vector<Enemy*> enmeiesP;
	for (auto& e : enemies) {
		enmeiesP.push_back(&e);
	}

	reCalculatePaths(enmeiesP);

	for (auto& e : enemies) {
		Cell* eLocation = &e.getCellLocation();
		Cell* nextTargetInPath = enemiesPaths[&e][eLocation];
		if (nextTargetInPath == nullptr)
			throw "enemy location isn't in path to target";

		e.setTarget(*nextTargetInPath);
	}
}

MazeGame::MazeGame(int size)
{
	solved = false;
	initalizeMaze(size);
	initalizePlayer();
	initalizePlayerEscapeBrain();
	initalizeEnemies();
	initalizeEnemiesBrain();
}

MazeGame::~MazeGame()
{
	delete maze;
	delete player;
	delete enemyBrain;
	delete playerEscapeBrain;
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

	if (!target) {
		vector<Cell*> neighbors = maze->getNeighbors(*current);
		target = neighbors[rand() % neighbors.size()];
	}

	o.setTarget(*target);
}

void MazeGame::updateEnemies()
{
	for (auto& e : enemies) {
		e.move(); 
		updateTargetLocation(e, enemiesPaths[&e]);
	}
	enemyBrainTick();
}

map<Cell*, Cell*> MazeGame::findCoinPath() 
{
	Cell& pLocation = player->getCellLocation();
	vector<Cell*> neighbors = maze->getNeighbors(pLocation);
	vector<Cell*> neighborsWithCoin;

	for (auto& n : neighbors) {
		if (n->getHasCoin()) {
			neighborsWithCoin.push_back(n);
		}
	}

	map<Cell*, Cell*> nextInPath;

	if (neighborsWithCoin.empty()) {
		if (neighbors.size() > 1) {
			Cell* pLocation = &player->getLastPlayerLoctaion();
			neighbors.erase(remove(neighbors.begin(), neighbors.end(), pLocation), neighbors.end());
		}
		
		nextInPath[&pLocation] = neighbors[rand() % neighbors.size()];
	}
	else {
		nextInPath[&pLocation] = neighborsWithCoin[rand() % neighborsWithCoin.size()];
	}
	return nextInPath;
}


vector<Cell*> MazeGame::calculatePossibleEscapePaths(Cell* c)
{

	/*
	 1. if u got more than 1 enemy in the radius
	  - check if there is a free path
	   - yes -> pick that path
	   - no -> take the long enemy path
	*/

	/*
		check if spesific neigbor is found in more than 1 diffreet path
		if it does check which path is longer
		and choose that path
	
	*/
	vector<Cell*> escapePaths;
	vector<Cell*> notDeadEnd;
	vector<Cell*> neighbors = maze->getNeighbors(*c);

	/* check if the neighbors of the cell is leading to dead end*/
	for (auto& n : neighbors) {
		if (!maze->isLeadingToDeadEnd(*c,*n,1)) {
			notDeadEnd.push_back(n);
		}
	}
	
	/* map of cell's neighbors that not leading to a dead end*/
	for (auto& n : notDeadEnd) {
		if (!n->getPath()) {
			escapePaths.push_back(n);
		}
	}

	if (!escapePaths.empty()) {
		return escapePaths;
	}

	if (!notDeadEnd.empty()) {
		return notDeadEnd;
	}

	return neighbors;
}

map<Cell*, Cell*> MazeGame::findEscapePath(vector<MazeMovingObj*> enemies)
{
	vector<Cell*> targets;
	for (auto& e : enemies) {
		Cell* eLocation = &(e->getCellLocation());
		targets.push_back(eLocation);
	}

	Cell& start = player->getCellLocation();
	playerEscapeBrain->setStartTarget(start, targets);
	playerEscapeBrain->solve();

	enemiesPathToPlayer[&start] = (playerEscapeBrain->getPathToTargets()); /*yoni*/

	vector<Cell*> paths = calculatePossibleEscapePaths(&start);
	map<Cell*, Cell*> nextInPath;

	/* paths = possibole escape paths
		check in paths if more than 1 neighbors is found in enemy path to player
		if more than one is found: choose the neighbors that found in the longest path to the player.
	*/


	nextInPath[&start] = paths[rand() % paths.size()];
	playerEscapeBrain->clear();

	return nextInPath;
}

vector<MazeMovingObj*> MazeGame::areEnemiesInRange()
{	
	vector<MazeMovingObj*> enemiesInRage;

	double maxDistance = player->getEnemySearchRadius();

	for (auto& e : enemies) {
		double dist = manhattan_distance((MovingObj*)player,(MovingObj*)&e);
		if(dist < maxDistance){
			enemiesInRage.push_back(&e);
		}
	}
	return enemiesInRage;
}

void MazeGame::updatePlayer()
{
	player->move();

	map<Cell*, Cell*> nextInPath;

	if (!player->isAtTarget())
		return;

	vector<MazeMovingObj*> enemies = areEnemiesInRange();
	
	if (enemies.empty()) {
		nextInPath = findCoinPath();
	}else {
		nextInPath = findEscapePath(enemies);
	}

	updateTargetLocation(*player, nextInPath);

}

bool MazeGame::isEnemyGotPlayer()
{
	/* TODO add smart solution for dynamic radius size */
	float radiusesSize = 2.0f * OpenGL::circleR * 1.2f * 4.0f;

	float px = player->getX();
	float py = player->getY();

	for (auto& e : enemies) {
		float ex = e.getX();
		float ey = e.getY();

		float xDiff = px - ex;
		float yDiff = py - ey;
		float dist = (float) sqrt(pow(xDiff, 2) + pow(yDiff, 2));
		if (dist < radiusesSize) {
			cout << "got hit" << endl;
			return true;
		}
	}

	return false;
}

void MazeGame::enemyBrainTick()
{	

	/*
	- iterate over enemies last tick
		- check if now - last tick < enemy amount of tick
			reutrn
		- add to vector of recalculate paths
	- recalcuate path to enemy requierd
	*/

	high_resolution_clock::time_point now = high_resolution_clock::now();
	//duration<double, std::milli> time_span = now - last_tick;

	vector <Enemy*> requireBrainUpdate;

	for (auto& e : enemies) {
		duration<double, std::milli> time_span = now - e.getLastTick();
		if (time_span.count() >= e.getBrainCalculationEachMs()) {
			e.setLastTick(now);
			requireBrainUpdate.push_back(&e);
		}
	}


	reCalculatePaths(requireBrainUpdate);

	/* 
	 * update target to return to the center of the current cell before continuing in their new path 
	 * this should prevent some buggy movement from the edge of the cell to the new cell	
	 */

	for (auto& e : requireBrainUpdate) {
		Cell& eLocation = e->getCellLocation();
		e->setTarget(eLocation);
	}
}


void MazeGame::solveIteration()
{
	if (solved)
		return;

	updateEnemies();
	
	updatePlayer();
	
	if (numCoins == 0) {
		solved = true;
		cout << "numCoins is zero " << endl;
	}

	if (isEnemyGotPlayer()) {
		solved = true;
		cout << "player got hit by enemy" << endl;
	}
	
}