#include <iostream>

#include "PathFinder.hpp"
#include "RoomPathFinder.hpp"
#include "Game.hpp"
#include "GamePoint.hpp"
#include "Corridor.hpp"
#include "Room.hpp"
#include "Cell.hpp"
#include "Bot.hpp"
#include "heruistics.h"
#include "RoomPathFinder.hpp"
#include "ConnectorPathFinder.hpp"
#include "Team.hpp"

using namespace std;

constexpr auto MIN_FIGHT_DIST = 2;
constexpr auto CORRIDOR_IGNORE_RADIUS = 10;

PathFinder::PathFinder() : cellMovingObject(nullptr), teams(Game::getGameTeams()),
rooms(Game::getGameRooms()), corridors(Game::getGameCorridors()) 
{
	roomFinder = new RoomPathFinder();
	connectorFinder = new ConnectorPathFinder();
}

PathFinder::PathFinder(CellMovingObject& cellMovingObject) : cellMovingObject(&cellMovingObject), teams(Game::getGameTeams()),
		rooms(Game::getGameRooms()), corridors(Game::getGameCorridors())
{
	roomFinder = new RoomPathFinder(cellMovingObject);
	connectorFinder = new ConnectorPathFinder(cellMovingObject);
}

void PathFinder::setSource(CellMovingObject& cellMovingObject)
{
	this->cellMovingObject = &cellMovingObject;
	roomFinder->setSource(cellMovingObject);
	connectorFinder->setSource(cellMovingObject);
}

Team* PathFinder::getEnemyTeam()
{
	for (auto& team : teams) {
		bool found = false;
		for (auto& bot : team->getBots()) {
			if (bot == cellMovingObject) {
				found = true;
			}
		}

		if (!found) {
			return team;
		}
	}

	return nullptr;
}

Bot* PathFinder::getClosestEnemy(Team& enemyTeam, bool& closeEnemy, float& minFireRange)
{
	float minDist = FLT_MAX;
	Bot* closestBot = nullptr;
	BoardCells& sb = cellMovingObject->getBoardCells();
	Cell& sc = cellMovingObject->getCellLocation();
	closeEnemy = false;

	float sx = sb.getXYOffset().x + sc.getX();
	float sy = sb.getXYOffset().y + sc.getY();

	for (auto& bot : enemyTeam.getBots()) {
		BoardCells& tb = bot->getBoardCells();
		Cell& tc = bot->getCellLocation();

		float tx = tb.getXYOffset().x + tc.getX();
		float ty = tb.getXYOffset().y + tc.getY();

		float dist = manhattan_distance(sx, sy, tx, ty);
		/* if enemy found in small radius return nullptr to activate roam*/

		float randMinFireDist = static_cast<float>((rand() % MIN_FIGHT_DIST) + 1);

		if (dist < randMinFireDist) {
			closeEnemy = true;
			return bot;
		}

		if (!tb.getShootable() && dist < CORRIDOR_IGNORE_RADIUS)
			continue;


		if (dist < minDist) {
			closestBot = bot;
			minDist = dist;
			minFireRange = randMinFireDist;
		}
	}

	return closestBot;
}

stack<GamePoint> PathFinder::roamWithDistancing(Bot& closetEnemy)
{
	BoardCells* b_ptr = &cellMovingObject->getBoardCells();
	Cell& c = cellMovingObject->getCellLocation();
	vector<Cell*> neighbors = getCellNeighbors(c);

	if (neighbors.size() <= 1) {
		return roam();
	}

	Cell& enemyCell = closetEnemy.getCellLocation();
	float minDist = FLT_MAX;
	Cell* closestCell = nullptr;

	for (auto& n : neighbors) {
		float distance = manhattan_distance(n, &enemyCell);
		if (distance < minDist) {
			minDist = distance;
			closestCell = n;
		}
	}

	/* allow for some randomness to avoid obstacle distance / closing routine*/
	if (rand() % 4 == 0) {
		neighbors.erase(remove(neighbors.begin(), neighbors.end(), closestCell), neighbors.end());
	}

	Cell* randomCell = neighbors[rand() % neighbors.size()];

	GamePoint target(b_ptr, randomCell);
	stack<GamePoint> path;
	path.push(target);

	return path;
}


stack<GamePoint> PathFinder::searchClosetEnemy()
{
	float minDistFire = 0;
	bool isEnemyTooClose;
	Team* enemyTeam = getEnemyTeam();
	if (enemyTeam == nullptr) {
		cout << __func__ << " something went wrong couldn't getEnemyTeam" << endl;
		stack<GamePoint> s;
		return s;
	}

	Bot* closetEnemy = getClosestEnemy(*enemyTeam, isEnemyTooClose, minDistFire);
	if (closetEnemy == nullptr) {
		return roam();
	}
	else if (isEnemyTooClose) {
		return roamWithDistancing(*closetEnemy);
	}

	/* per design if enemy and bot in the same room return target cell and null for board */
	if (&closetEnemy->getBoardCells() == &cellMovingObject->getBoardCells()) {
		stack<GamePoint> path;
		GamePoint gp(nullptr, &closetEnemy->getCellLocation());
		float dist = manhattan_distance(cellMovingObject, static_cast<CellMovingObject*>(closetEnemy));
		float midRangeAllowedDist = static_cast<float>((rand() % 10) + 2);
		if (dist > minDistFire + midRangeAllowedDist) {
			gp.board = &cellMovingObject->getBoardCells();
		}
		path.push(gp);
		return path;
	}
	GamePoint targetPoint(&closetEnemy->getBoardCells(), &closetEnemy->getCellLocation());

	return connectorFinder->getPath(targetPoint);
}

stack<GamePoint> PathFinder::searchTeammate(Bot& teammate)
{
	GamePoint targetPoint(&teammate.getBoardCells(), &teammate.getCellLocation());

	if (&teammate.getBoardCells() == &cellMovingObject->getBoardCells()) {
		stack<GamePoint> path;
		targetPoint.board = nullptr;
		path.push(targetPoint);
		return path;
	}

	return connectorFinder->getPath(targetPoint);
}

GamePoint PathFinder::findClosetConsumable(ConsumableType type)
{
	BoardCells* targetBoard = nullptr;
	Cell* targetCell = nullptr;
	float minDist = FLT_MAX;

	/* calculate src x,y*/
	BoardCells& sb = cellMovingObject->getBoardCells();
	Cell& sc = cellMovingObject->getCellLocation();

	vec2f srcLocation = cellMovingObject->getLocation();

	float sx = srcLocation.x;
	float sy = srcLocation.y;

	/* iterate on all room and all consumables based on type in each room*/
	vector<Consumable*> consumables;
	for (auto& r : rooms) {
		if (type == ConsumableType::AMMO) {
			consumables = r->getAmmoBoxes();
		}
		else {
			consumables = r->getHealthBoxes();
		}

		for (auto& consumable : consumables) {
			if (consumable->getHidden())
				continue;

			vec2f& tXY = r->getXYOffset();
			Cell& tc = *consumable->getLocation();

			float tx = tXY.x + tc.getX() + 0.5f;
			float ty = tXY.y + tc.getY() + 0.5f;

			float dist = manhattan_distance(sx, sy, tx, ty);
			if (dist < minDist) {
				minDist = dist;
				targetBoard = r;
				targetCell = &tc;
			}
		}
	}

	/* return nullptr if in the same room */
	if (targetBoard == &sb) {
		return GamePoint(nullptr, targetCell);
	}

	return GamePoint(targetBoard, targetCell); 
}

stack<GamePoint> PathFinder::searchClosetConsumable(ConsumableType type)
{
	stack<GamePoint> path;
	GamePoint closestConsumable = findClosetConsumable(type);
	if (closestConsumable.board && closestConsumable.cell) {
		return connectorFinder->getPath(closestConsumable);
	}

	/* board nullptr and cell exist means consumable is at the same room */
	if (!closestConsumable.board && closestConsumable.cell) {
		path.push(closestConsumable);
	}

	return path;
}

stack<GamePoint> PathFinder::searchClosetAmmo()
{
	return searchClosetConsumable(ConsumableType::AMMO);
}


stack<GamePoint> PathFinder::searchClosetHealth()
{
	return searchClosetConsumable(ConsumableType::HEALTH);

}

vector<Cell*> PathFinder::getCellNeighbors(Cell& cell)
{
	vector<Cell*> neighbors;
	CellMat& cells = cellMovingObject->getBoardCells().getCells();
	/* add -1 to accomidate the fact that array start from index 0 */
	int width = cells[0].size() - 1;
	int height = cells.size() - 1;

	int x = cell.getX();
	int y = cell.getY();
	int offsets[][2] = {
		{x + 1, y},
		{x - 1, y},
		{x, y + 1},
		{x, y - 1}
	};

	for (auto& offset : offsets) {
		int nx = offset[0];
		int ny = offset[1];

		if (nx < 0 || nx > width) {
			continue;
		}
		else if (ny < 0 || ny > height) {
			continue;
		}

		Cell* neighbor = &cells[ny][nx];

		if (!neighbor->getIsOccupy()) {
			neighbors.push_back(neighbor);
		}
	}

	return neighbors;
}

stack<GamePoint> PathFinder::roam()
{
	Cell* neighborInAnotherBoard = nullptr;
	BoardCells* AnotherBoard = nullptr;

	BoardCells& srcBoard = cellMovingObject->getBoardCells();
	Cell& c = cellMovingObject->getCellLocation();
	vector<Cell*> neighbors = getCellNeighbors(c);
	BoardCells* targetBoard = &srcBoard;

	/* add connector cell in another board if cell is a conencting cell  */
	vector<BoardCells*> connectedBoards = srcBoard.getConnectors();
	for (auto& board : connectedBoards) {
		Cell* connector = srcBoard.getConnectingCell(*board);
		if (&c == connector) {
			neighborInAnotherBoard = board->getConnectingCell(srcBoard);
			if (neighborInAnotherBoard) {
				neighbors.push_back(neighborInAnotherBoard);
				AnotherBoard = board;
				break;
			}
		}
	}

	Cell* randomTarget = neighbors[rand() % neighbors.size()];
	if (randomTarget == neighborInAnotherBoard) {
		targetBoard = AnotherBoard;
	}

	GamePoint target(targetBoard, randomTarget);
	stack<GamePoint> path;
	path.push(target);

	return path;
}

map<Cell*, Cell*> PathFinder::getRoomPath(Cell* target)
{
	if (target == &cellMovingObject->getCellLocation()) {
		map<Cell*, Cell*> path;
		path[target] = target;
		return path;
	}

	return roomFinder->getPath(target);
}

void PathFinder::drawGraph()
{
	connectorFinder->draw();
}
void PathFinder::drawRoomPath()
{
	roomFinder->draw();
}