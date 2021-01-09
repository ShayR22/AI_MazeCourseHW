#include "MazeGame.h"
#include "OpenGL.h"
#include "heruistics.h"
#include <map>
#include <array>

using namespace std;
using namespace std::chrono;

void MazeGame::initalizeMaze(int size)
{
	numCoins = size * size;
	maze = new Maze(size, size);
	maze->setColorHidden(false);

	start = maze->getStarts()[0];

	cell_mat& cells = maze->getCells();
	for (auto& cellsRow : cells)
		for (auto& cell : cellsRow)
			cell.setHasCoin(true);
}

void MazeGame::initalizePlayer()
{
	constexpr auto PLAYER_MOVEMENT_PER_FRAME = 1.25f;

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
			cout << "Coins: before remove: size = " << neighbors.size() << endl;
			Cell* pLocation = &player->getLastPlayerLoctaion();
			neighbors.erase(remove(neighbors.begin(), neighbors.end(), pLocation), neighbors.end());
			cout << "Coins: after remove: size = " << neighbors.size() << endl;
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

	/* save walls of enemies aside and set all their walls to true,
	   this would make the algorithm remove a path direct to them, as
	   if it is a dead end
	*/

	//bool allWalls[4] = { true, true, true, true };
	//for (auto& e : enemies)
	//	e.getCellLocation().setWalls(allWalls);

	/* check if the neighbors of the cell is leading to dead end*/
	for (auto& n : neighbors) {
		if (!maze->isLeadingToDeadEnd(*c,*n,1)) {
			notDeadEnd.push_back(n);
		}
	}

	//for (auto& e : enemies)
	//	e.getCellLocation().wallRestoration();

	
	/* map of cell's neighbors that not leading to a dead end*/
	for (auto& n : notDeadEnd) {
		if (!n->getPath()) {
			escapePaths.push_back(n);
		}
	}

	if (!escapePaths.empty()) {
		cout << "returning escapPaths size: " << escapePaths.size() << endl;
		return escapePaths;
	}

	vector <vector< Cell* >> pathsToEnemies = enemiesPathToPlayer[&player->getCellLocation()];
	cout << "no escape paths" << endl;
	if (pathsToEnemies.empty()) {
		cout << "paths to enemies is empty" << endl;
		return neighbors;
	}

	unsigned int maxSize = 0;
	vector<Cell*> *longestPath = nullptr;
	for (auto& p : pathsToEnemies) {
		if (p.size() > maxSize) {
			longestPath = &p;
			maxSize = p.size();
		}
	}

	if (!longestPath || longestPath->empty()) {
		if (notDeadEnd.empty())
			return neighbors;
		return notDeadEnd;
	}

	vector<Cell*> cellWithLongestEnemyPath;
	Cell* nextTarget = (*longestPath)[longestPath->size() - 2];

	if (nextTarget == &player->getLastPlayerLoctaion()) {
		vector<Cell*>& randomPath = pathsToEnemies[rand() % pathsToEnemies.size()];
		Cell* randomPick = randomPath[randomPath.size() - 2];
		cellWithLongestEnemyPath.push_back(randomPick);
	}
	else {
		cellWithLongestEnemyPath.push_back(nextTarget);
	}

	return cellWithLongestEnemyPath;

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
	if (paths.empty()) {
		cout << "Warning no path could be calculated " << endl;
	}
	else {
		nextInPath[&start] = paths[rand() % paths.size()];
	}

	//playerEscapeBrain->clear();


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

	constexpr auto PLAYER_BRAIN_CALC_MS = 1;
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	duration<double, std::milli> time_span = now - player->getLastTick();


	if (time_span.count() < PLAYER_BRAIN_CALC_MS) {
		nextInPath = findCoinPath();
		updateTargetLocation(*player, nextInPath);
		player->updateLastLocation();
		return;

	}

	player->setLastTick(now);

	vector<MazeMovingObj*> enemies = areEnemiesInRange();
	
	if (enemies.empty()) {
		cout << "no enemies in range" << endl;
		nextInPath = findCoinPath();
	}else {
		cout << "enemies in range" << endl;
		nextInPath = findEscapePath(enemies);
	}

	if (!nextInPath.empty())
		updateTargetLocation(*player, nextInPath);

	player->updateLastLocation();
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