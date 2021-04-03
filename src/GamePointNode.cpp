#include "GamePoint.hpp"
#include "GamePointNode.hpp"
#include "GamePointEdge.hpp"
#include <iostream>

using namespace std;

GamePointNode::GamePointNode(GamePoint& gameLocation) : gameLocation(gameLocation),
visited(false), isVisiting(false), isPath(false), parent(nullptr),
xy({ gameLocation.getAbsoluteX(), gameLocation.getAbsoluteY() }) {}

GamePointNode::GamePointNode(const GamePointNode& other) : gameLocation(other.gameLocation),
xy(other.xy), edges(other.edges), visited(other.visited), isVisiting(other.isVisiting),
isPath(other.isPath), parent(other.parent) {}

void GamePointNode::addEdge(GamePointEdge& edge)
{
    edges.push_back(&edge);
}

void GamePointNode::removeEdge(GamePointEdge& edge)
{
    edges.erase(remove(edges.begin(), edges.end(), &edge), edges.end());
}

vector<GamePointNode*> GamePointNode::getNieghbors()
{
    vector<GamePointNode*> neighbors;

    for (unsigned int i = 0; i < edges.size(); i++)
        neighbors.push_back(edges[i]->getTo());

    return neighbors;
}

