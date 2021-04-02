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
		for (auto& bot : team.getBots()) {
			if (bot == cellMovingObject) {
				found = true;
			}
		}

		if (!found) {
			return &team;
		}
	}

	return nullptr;
}

Bot* PathFinder::getClosestEnemy(Team& enemyTeam)
{
	float minDist = FLT_MAX;
	Bot* closestBot = nullptr;
	BoardCells& sb = cellMovingObject->getBoardCells();
	Cell& sc = cellMovingObject->getCellLocation();

	float sx = sb.getXYOffset().x + sc.getX();
	float sy = sb.getXYOffset().y + sc.getY();

	for (auto& bot : enemyTeam.getBots()) {
		BoardCells& tb = bot->getBoardCells();
		Cell& tc = bot->getCellLocation();

		float tx = tb.getXYOffset().x + tc.getX();
		float ty = tb.getXYOffset().y + tc.getY();

		float dist = manhattan_distance(sx, sy, tx, ty);
		if (dist < minDist) {
			closestBot = bot;
			minDist = dist;
		}
	}

	return closestBot;
}

stack<GamePoint> PathFinder::searchClosetEnemy()
{
	
	Team* enemyTeam = getEnemyTeam();
	if (enemyTeam == nullptr) {
		cout << __func__ << " something went wrong couldn't getEnemyTeam" << endl;
		stack<GamePoint> s;
		return s;
	}

	Bot* closetEnemy = getClosestEnemy(*enemyTeam);
	if (closetEnemy == nullptr) {
		cout << __func__ << " something went wrong couldn't find getClosestEnemy" << endl;
		stack<GamePoint> s;
		return s;
	}

	GamePoint targetPoint(&closetEnemy->getBoardCells(), &closetEnemy->getCellLocation());

	return connectorFinder->getPath(targetPoint);
}

stack<GamePoint> PathFinder::searchTeammate(Bot& teammate)
{
	GamePoint targetPoint(&teammate.getBoardCells(), &teammate.getCellLocation());

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

	float sx = sb.getXYOffset().x + sc.getX();
	float sy = sb.getXYOffset().y + sc.getY();

	/* iterate on all room and all consumables based on type in each room*/
	vector<Consumable*> consumables;
	for (auto& r : rooms) {
		if (type == ConsumableType::AMMO) {
			consumables = r.getAmmoBoxes();
		}
		else {
			consumables = r.getHealthBoxes();
		}

		for (auto& consumable : consumables) {
			vec2f& tXY = r.getXYOffset();
			Cell& tc = *consumable->getLocation();
			float tx = tXY.x + tc.getX();
			float ty = tXY.y + tc.getY();

			float dist = manhattan_distance(sx, sy, tx, ty);
			if (dist < minDist) {
				minDist = dist;
				targetBoard = &r;
				targetCell = &tc;
			}
		}
	}

	return GamePoint(targetBoard, targetCell); 
}

stack<GamePoint> PathFinder::searchClosetAmmo()
{
	GamePoint closestAmmo = findClosetConsumable(ConsumableType::AMMO);
	if (!closestAmmo.board || !closestAmmo.cell) {
		stack<GamePoint> empty;
		return empty;
	}

	return connectorFinder->getPath(closestAmmo);
}

stack<GamePoint> PathFinder::searchClosetHealth()
{
	GamePoint closestAmmo = findClosetConsumable(ConsumableType::AMMO);
	if (!closestAmmo.board || !closestAmmo.cell) {
		stack<GamePoint> empty;
		return empty;
	}

	return connectorFinder->getPath(closestAmmo);
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
	BoardCells& srcBoard = cellMovingObject->getBoardCells();
	Cell& c = cellMovingObject->getCellLocation();
	vector<Cell*> neighbors = getCellNeighbors(c);

	BoardCells* targetBoard = &srcBoard;

	/* add connector cell in another board if cell is a conencting cell  */
	vector<BoardCells*> connectedBoards = srcBoard.getConnectors();
	for (auto& board : connectedBoards) {
		Cell* connector = srcBoard.getConnectingCell(*board);
		if (&c == connector) {
			Cell* neighborInAnotherBoard = board->getConnectingCell(srcBoard);
			if (neighborInAnotherBoard)
				neighbors.push_back(neighborInAnotherBoard);

			targetBoard = board;
			break;
		}
	}

	Cell* randomTarget = neighbors[rand() % neighbors.size()];

	GamePoint target(targetBoard, randomTarget);
	stack<GamePoint> path;
	path.push(target);

	return path;
}

std::map<Cell*, Cell*> PathFinder::getRoomPath(Cell* target)
{
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