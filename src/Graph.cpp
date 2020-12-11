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
	while (--i >= 0) {
		if (cm[i + 1][j].getWallTop())
			return false;
		if (nm[i][j] != nullptr)
			return true;
		
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
	return !(mat[i][j].getWallLeft() && mat[i][j].getWallRight());
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

void Graph::findCreateEdge(int i, int j, node_mat& nm, cell_mat& cm)
{
	int row = i;
	int col = j;

	/* search right */
	while (!cm[i][col].getWallRight() && nm[i][++col] == nullptr);

	if (nm[i][col] != nullptr && col != j) {
		edges.push_back(new Edge(nm[i][j], nm[i][col]));
		nm[i][j]->addEdge(*edges.back());
		edges.push_back(new Edge(nm[i][col], nm[i][j]));
		nm[i][col]->addEdge(*edges.back());
	}

	/* search down */
	while (!cm[row][j].getWallDown() && nm[++row][j] == nullptr);

	if (nm[row][j] != nullptr && row != i) {
		edges.push_back(new Edge(nm[i][j], nm[row][j]));
		nm[i][j]->addEdge(*edges.back());
		edges.push_back(new Edge(nm[row][j], nm[i][j]));
		nm[row][j]->addEdge(*edges.back());
	}
}


Graph::Graph(Maze& maze)
{
	node_mat nm{};
	cell_mat& cm = maze.getCells();

	initalizeNodeMat(nm, cm);
	setStartsAndTarget(nm, maze);

	int numRows = cm.size();
	int numCols = cm[0].size();
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
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

	for (unsigned int i = 0; i < nm.size(); i++) {
		for (unsigned int j = 0; j < nm[0].size(); j++) {
			if (nm[i][j] == nullptr)
				continue;
			findCreateEdge(i, j, nm, cm);
		}
	}


	removeNoWhereToGo();
}

void Graph::removeNoWhereToGo() noexcept(false)
{
	for (unsigned int i = 0; i < starts.size(); i++)
		if (starts[i]->getNieghbors().size() == 0)
			throw "start has no nodes connected to it";

	if (target->getNieghbors().size() == 0)
		throw "target has no nodes connected to it";

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
