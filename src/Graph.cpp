#include "Graph.h"

using namespace std;


void Graph::initalizeNodeMat(node_mat& nm, cell_mat& cm)
{
	int numRows = cm.size();
	int numCols = cm[0].size();

	nm.reserve(numRows);
	vector<Node*> cur_row;
	for (int i = 0; i < numRows; i++) {
		cur_row.clear();
		for (int j = 0; j < numCols; j++)
			cur_row.push_back(nullptr);

		nm.push_back(cur_row);
	}
}

void Graph::setStartsAndTarget(node_mat& nm, Maze& maze)
{

	vector<Cell*> cellStarts = maze.getStarts();
	for (unsigned int i = 0; i < cellStarts.size(); i++) {
		int row = cellStarts[i]->getRow();
		int col = cellStarts[i]->getCol();

		nodes.push_back(new Node(row, col));
		nm[row][col] = nodes.back();
		starts.push_back(nodes.back());
	}

	Cell* cellTarget = &maze.getTarget();
	nodes.push_back(new Node(cellTarget->getRow(), cellTarget->getCol()));
	nm[cellTarget->getRow()][cellTarget->getCol()] = nodes.back();
	target = nodes.back();
}


bool Graph::isAbove(int i, int j, node_mat& nm, cell_mat& cm)
{
	i++;
	while (i >= 0) {
		if (nm[i][j] != nullptr)
			return true;
		if (cm[i][j].getWallTop())
			return false;

		i--;
	}
	return false;
}

bool Graph::isBelow(int i, int j, cell_mat& mat)
{
	return !mat[i][j].getWallDown();
}

bool Graph::isDeadEnd(int i, int j, cell_mat& mat)
{
	bool* walls = mat[i][j].getWalls();
	int counter = 0;
	
	for (int i = 0; i < 4; i++)
		if (walls[i])
			counter++;

	return (counter >= 3);
}

bool Graph::isSideways(int i, int j, cell_mat& mat)
{
	return (!mat[i][j].getWallLeft() || !mat[i][j].getWallRight());
}

bool Graph::toCreateNode(int i, int j, node_mat& nm, cell_mat& cm)
{
	if (isDeadEnd(i, j, cm))
		return false;
	if (isBelow(i, j, cm) && isSideways(i, j, cm))
		return true;
	if (!isBelow(i, j, cm) && isAbove(i, j, nm, cm))
		return true;

	return false;
}

void Graph::findCreateNode(int i, int j, node_mat& nm, cell_mat& cm)
{
	int row = i + 1;
	int col = j + 1;

	/* search right */
	while (nm[i][col] == nullptr && !cm[i][col].getWallRight())
		col++;

	if (nm[i][col] != nullptr) {
		edges.push_back(Edge(nm[i][j], nm[i][col]));
		nm[i][j]->addEdge(edges.front());
		edges.push_back(Edge(nm[i][col], nm[i][j]));
		nm[i][col]->addEdge(edges.front());
	}

	/* search down */
	while (nm[row][j] == nullptr && !cm[row][j].getWallDown())
		row++;

	if (nm[row][j] != nullptr) {
		edges.push_back(Edge(nm[i][j], nm[row][j]));
		nm[i][j]->addEdge(edges.front());
		edges.push_back(Edge(nm[row][j], nm[i][j]));
		nm[row][j]->addEdge(edges.front());
	}
}


Graph::Graph(Maze& maze)
{
	node_mat nm{};
	cell_mat& cm = maze.getCells();

	initalizeNodeMat(nm, cm);
	setStartsAndTarget(nm, maze);
	cout << "Finished initNode mat and set StartsTargets" << endl;

	int numRows = cm.size();
	int numCols = cm[0].size();
	for (int i = 1; i < numRows - 1; i++) {
		for (int j = 1; j < numCols - 1; j++) {
			if (nm[i][j] != nullptr)
				continue;

			if (toCreateNode(i, j, nm, cm)) {
				int row = cm[i][j].getRow();
				int col = cm[i][j].getCol();

				nodes.push_back(new Node(row, col));
				nm[i][j] = nodes.back();
			}
		}
	}
	cout << "Finished creating nodes" << endl;

	for (unsigned int i = 0; i < nm.size(); i++) {
		for (unsigned int j = 0; j < nm[0].size(); j++) {
			if (nm[i][j] == nullptr)
				continue;
			findCreateNode(i, j, nm, cm);
		}
	}
	cout << "Finished creating edges" << endl;



	removeNoWhereToGo();
	setHeuristics();
}

void Graph::removeNoWhereToGo() noexcept(false)
{
	for (unsigned int i = 0; i < starts.size(); i++)
		if (starts[i]->getNieghbors().size() == 0)
			throw "start has no nodes connected to it";

	if (target->getNieghbors().size() == 0)
		throw "target has no nodes connected to it";

	cout << "going to over nodes" << endl;
	int counter = 0;
	int nodesSize = (int)nodes.size();
	auto itr = nodes.begin();
	while (itr != nodes.end()) {
		if ((*itr)->getNieghbors().size() == 0) {
			counter++;
			Node* tmp = *itr;
			itr = nodes.erase(itr);
			delete tmp;
		} else {
			++itr;
		}
	}

	cout << "removed " << ((float)counter)/nodesSize << "% of nodes" << endl;
}


void Graph::setHeuristics()
{
	int targetX = target->getX();
	int targetY = target->getY();

	for (auto& n : nodes)
	{
		int nX = n->getX();
		int nY = n->getY();
		
		double distance = sqrt(pow((double)targetX - nX, 2) + pow((double)targetY - nY, 2));
		n->setHeuristic(distance);
	}
}

void Graph::draw()
{
	Edge::drawEdges(edges);
	Node::drawPoints(nodes);

	Cell::setOpenGLStartColor();
	for (unsigned int i = 0; i < starts.size(); i++) {
		starts[i]->draw();
	}

	Cell::setOpenGLTargetColor();
	target->draw();
}

Graph::~Graph()
{
	for (unsigned int i = 0; i < nodes.size(); i++)
		delete nodes[i];
}
