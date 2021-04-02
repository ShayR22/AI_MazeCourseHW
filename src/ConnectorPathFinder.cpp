#include <iostream>

#include "Drawer.hpp"
#include "ConnectorPathFinder.hpp"
#include "GamePoint.hpp"
#include "GamePointEdge.hpp"
#include "Game.hpp"
#include "Team.hpp"
#include "Bot.hpp"
#include "Room.hpp"
#include "Corridor.hpp"
#include "heruistics.h"

using namespace std;

void ConnectorPathFinder::clear()
{
	openSet.clear();
	closedSet.clear();

	for (auto& n : nodes) {
		heuristicCost[n] = manhattan_distance(n, target);;
		moveCost[n] = INT_MAX;
		funcValues[n] = INT_MAX;

		n->setIsPath(false);
		n->setIsVisiting(false);
		n->setVisited(false);
		n->setParent(nullptr);
	}

	moveCost[start] = 0;
	funcValues[start] = heuristicCost[start] + moveCost[start];

	openSet.insert(start);
}


void ConnectorPathFinder::generateNodesAndEdges()
{
	/*
	 - iterate on corridros
	 - iterate on corridor's rooms connections
	 - create node from corridor cell
	 - extract connected room
	 - find in room the cell that is connected to the corridor's cell
	 - create node for the room's cell
	 - create an edges between the two
	*/

	/* iterate on corridros */
	for (auto& corridor : corridors) {
		vec2f& cXY = corridor.getXYOffset();
		/* iterate on corrdiror rooms */
		for (auto& cr : corridor.getRoomConnections()) {

			/* create first node from the corridor*/
			Cell& c = *cr.first;

			GamePoint tempGamePoint1(&corridor, &c);
			nodes.push_back(new GamePointNode(tempGamePoint1));
			GamePointNode* gpn1 = nodes.back();
			Room* r = cr.second;

			/* find in room the cell that is connected to the corridor's cell */
			Cell* rCell = r->getConnectingCell(corridor);
			if (rCell == nullptr) {
				cout << "something went wrong couldn't find connecting cell of room" << endl;
				return;
			}

			/* create node for the room's cell */
			GamePoint tempGamePoint2(r, rCell);
			nodes.push_back(new GamePointNode(tempGamePoint2));
			GamePointNode* gpn2 = nodes.back();

			/* create an edges between the two */
			edges.push_back(new GamePointEdge(gpn1, gpn2));
			gpn1->addEdge(*edges.back());

			edges.push_back(new GamePointEdge(gpn2, gpn1));
			gpn2->addEdge(*edges.back());
		}
	}

	/* create edges between same boardCells connectors */
	for (auto& n : nodes) {
		for (auto& nn : nodes) {
			GamePoint& nGL = n->getGameLocation();
			GamePoint& nnGL = nn->getGameLocation();

			/* exit if the same location or not the same board*/
			if (nGL == nnGL || nGL.board != nnGL.board) {
				continue;
			}

			/* create an edges between the two */
			edges.push_back(new GamePointEdge(n, nn));
			n->addEdge(*edges.back());

			/*edges.push_back(new GamePointEdge(nn, n));
			nn->addEdge(*edges.back());*/
		}
	}
}

ConnectorPathFinder::ConnectorPathFinder() : cellMovingObject(nullptr), teams(Game::getGameTeams()),
rooms(Game::getGameRooms()), corridors(Game::getGameCorridors()), start(nullptr), target(nullptr)
{
	generateNodesAndEdges();
}

ConnectorPathFinder::ConnectorPathFinder(CellMovingObject& cellMovingObject) : cellMovingObject(&cellMovingObject), teams(Game::getGameTeams()),
rooms(Game::getGameRooms()), corridors(Game::getGameCorridors()), start(nullptr), target(nullptr)
{
	generateNodesAndEdges();
}


void ConnectorPathFinder::removeStartTarget()
{
	nodes.erase(std::remove(nodes.begin(), nodes.end(), start), nodes.end());
	nodes.erase(std::remove(nodes.begin(), nodes.end(), target), nodes.end());

	delete start;
	delete target;

	for (auto& edge : startEdges) {
		edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
	}
	startEdges.clear();

	for (auto& edge : targetEdges) {
		edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
	}
	targetEdges.clear();
}

bool ConnectorPathFinder::isInNodes(GamePoint& point)
{
	for (auto& n : nodes) {
		vec2f& nXY = n->getXY();
		if (point.getAbsoluteX() == nXY.x && point.getAbsoluteY() == nXY.y) {
			return true;
		}
	}
	return false;
}

void ConnectorPathFinder::addStartTarget(GamePoint& gamePointTarget)
{
	/*
	 * if startPoint isn't an existing 
	 * create start
	 * create edges from start
	 * add edges to nodes from the start
	 */
	BoardCells& startB = cellMovingObject->getBoardCells();
	Cell& startC = cellMovingObject->getCellLocation();
	GamePoint startPoint(&startB, &startC);
	if (!isInNodes(startPoint)) {
		start = new GamePointNode(startPoint);
		for (auto& n : nodes) {
			if (n->getGameLocation().board != &startB) {
				continue;
			}
			
			startEdges.push_back(new GamePointEdge(start, n));
			GamePointEdge* edge = startEdges.back();
			edges.push_back(edge);
			start->addEdge(*edge);
		}
	}


	/*
	 * create target
	 * create edges to target
	 * add edges from nodes to the target
	 */
	 /* verify start isn't already a node (meaning on a connector) */

	if (!isInNodes(gamePointTarget)) {
		target = new GamePointNode(gamePointTarget);
		for (auto& n : nodes) {
			if (n->getGameLocation().board != target->getGameLocation().board) {
				continue;
			}

			targetEdges.push_back(new GamePointEdge(n, target));
			GamePointEdge* edge = targetEdges.back();
			edges.push_back(edge);
			n->addEdge(*edge);
		}
	}

	if (start)
		nodes.push_back(start);
	if (target)
		nodes.push_back(target);
}


GamePointNode* ConnectorPathFinder::findNeighborWithLowestFuncValue()
{
	/* add + 1 for edge case in start where all values are INT_MAX */
	double minVal = (double)INT_MAX + 1;
	GamePointNode* lowestNeighborF = nullptr;

	for (auto& n : openSet) {
		double curfVal = funcValues[n];
		if (curfVal < minVal) {
			minVal = curfVal;
			lowestNeighborF = n;
		}
	}

	return lowestNeighborF;
}

void ConnectorPathFinder::calculateStepInPathFromCurrentNode(GamePointNode* lowestF)
{
	vector<GamePointEdge*>& edges = lowestF->getEdges();
	for (auto& e : edges) {
		GamePointNode* neighbor = e->getTo();
		if (closedSet.find(neighbor) != closedSet.end())
			continue;

		double currentNeighborCost = moveCost[neighbor];
		double costFromNodeToNeighbor = moveCost[lowestF] + e->weight;

		/* if not in openSet add it and set neighbor to visiting */
		if (openSet.find(neighbor) == openSet.end()) {
			openSet.insert(neighbor);
			neighbor->setIsVisiting(true);
		}
		else if (currentNeighborCost < costFromNodeToNeighbor) {
			continue;
		}

		/* if not continued meaning need to update maps costs */
		moveCost[neighbor] = costFromNodeToNeighbor;
		funcValues[neighbor] = moveCost[neighbor] + heuristicCost[neighbor];
		neighbor->setParent(lowestF);
	}
}

stack<GamePoint> ConnectorPathFinder::generatePath()
{
	stack<GamePoint> path;
	GamePointNode* temp = target;
	while (temp != nullptr)
	{
		path.push(temp->getGameLocation());
		temp = temp->getParent();
	}

	return path;
}

stack<GamePoint> ConnectorPathFinder::getPath(GamePoint &gamePointTarget)
{
	addStartTarget(gamePointTarget);
	clear();

	while (!openSet.empty()) {
		GamePointNode* lowestF = findNeighborWithLowestFuncValue();
		openSet.erase(lowestF);
		lowestF->setIsVisiting(false);
		closedSet.insert(lowestF);
		lowestF->setVisited(true);

		if (lowestF == target) {
			std::cout << "solved" << std::endl;
			break;
		}

		calculateStepInPathFromCurrentNode(lowestF);
	}
		
	stack<GamePoint> path = generatePath();

	removeStartTarget();
	return path;
}

void ConnectorPathFinder::draw()
{
	/* draw nodes */
	for (auto& n : nodes) {
		BoardCells* b = n->getGameLocation().board;
		Cell* c = n->getGameLocation().cell;

		/* add 0.5 in order to be in the center of the cell */
		float x = b->getXYOffset().x + c->getX() + 0.5f;
		float y = b->getXYOffset().y + c->getY() + 0.5f;

		Drawer::filledCircle(x, y, 0.5f, DrawerColor::GREEN);
	}

	/* draw edges */
	for (auto& e : edges) {
		float sx = e->getFrom()->getGameLocation().getAbsoluteX() + 0.5f;
		float sy = e->getFrom()->getGameLocation().getAbsoluteY() + 0.5f;

		float tx = e->getTo()->getGameLocation().getAbsoluteX() + 0.5f;
		float ty = e->getTo()->getGameLocation().getAbsoluteY() + 0.5f;

		/* separate edges color, by if to the same boardcells in order to distinguish*/
		if (e->getFrom()->getGameLocation().board == e->getTo()->getGameLocation().board) {
			Drawer::line(sx, sy, tx, ty, DrawerColor::RED);
		}
		else {
			Drawer::line(sx, sy, tx, ty, DrawerColor::BLUE);
		}
	}
}

