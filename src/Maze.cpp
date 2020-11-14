#include <iostream>
#include <stack>
#include "glut.h"
#include "Maze.h"
#include <OpenGL.h>

using namespace std;

static inline void drawCell(double x, double y, double sx, double sy)
{
	glBegin(GL_POLYGON);  // fill up
	glVertex2d(x, y);
	glVertex2d(x, y + sy);
	glVertex2d(x + sx, y + sy);
	glVertex2d(x + sx, y);
	glEnd();
	glColor3d(0, 0, 0);
}

void Maze::allocateMaze(int numRows, int numCols)
{
	cells.reserve(numRows);
	vector<Cell> cur_row;
	for (int i = 0; i < numRows; i++) {
		cur_row.clear();
		for (int j = 0; j < numCols; j++)
			cur_row.push_back(Cell(i, j, CellState::space));

		cells.push_back(cur_row);
	}
}

void Maze::getNeighbors(Cell* c, Cell** neighbors, int *numNeighbors)
{
	/* set all to null*/

	int row = c->getRow();
	int col = c->getCol();

	/* left top right down */
	int indecies[4][2] = {
		{row, col - 1},
		{row - 1, col},
		{row, col + 1},
		{row + 1, col}
	};

	*numNeighbors = 0;
	int numRows = (int)cells.size();
	int numCols = (int)cells[0].size();
	for (int i = 0; i < 4; i++) {
		neighbors[i] = nullptr;
		int y = indecies[i][0];
		int x = indecies[i][1];
		if (y > numRows - 1 || y < 0)
			continue;
		if (x > numCols - 1 || x < 0)
			continue;;
		if (cells[y][x].getVisited())
			continue;

		(*numNeighbors)++;
		neighbors[i] = &cells[y][x];
	}
}

/* remove wall from to to by neighbor index*/
void Maze::removeWall(Cell* c, Cell* neighbor, int index)
{
	/* neighbors: left top right down */
	switch (index) {
	case 0:
		c->setWallLeft(false);
		neighbor->setWallRight(false);
		break;
	case 1:
		c->setWallTop(false);
		neighbor->setWallDown(false);
		break;
	case 2:
		c->setWallRight(false);
		neighbor->setWallLeft(false);
		break;
	case 3:
		c->setWallDown(false);
		neighbor->setWallTop(false);
		break;
	default:
		cout << __func__ << ": invalid index at removeWalls" << endl;
	}
}

static bool failChance()
{
	auto START_CHANCE = 0.6;
	constexpr auto MULTIPLIER = 0.85;
	static float chance_percent = START_CHANCE;

	if ((rand() % 100) / 100.0 < chance_percent) {
		chance_percent *= MULTIPLIER;
		return false;
	}

<<<<<<< HEAD
	for (int i = 1; i < size - 1; i++)
		for (int j = 1; j < size - 1; j++)
		{
			if (i % 2 == 1)  // this is mostly SPACE
			{
				if (rand() % 100 < 10) // 10% WALLs
					cells[i][j].setState(CellState::wall);
				else
					cells[i][j].setState(CellState::space);
			}
			else // this is mostly WALL
			{
				if (rand() % 100 < 60) // 60% SPACEs
					cells[i][j].setState(CellState::space);
				else
					cells[i][j].setState(CellState::wall);
			}
=======
	chance_percent = START_CHANCE;
	return true;
}

Cell* Maze::getPrevNeighborDirection(Cell** neighbors, int prevIndex)
{
	constexpr auto CONTINUE_FACTOR = 0.9;
	if (prevIndex == -1 || neighbors[prevIndex] == nullptr || failChance())
		return nullptr;
	return neighbors[prevIndex];
}

Cell* Maze::randomizeNeighbor(Cell** neighbors, int numNeighbors, int *neighborIndex)
{
	*neighborIndex = -1;
	/* select random neighbor */
	int randomized = rand() % numNeighbors;
	for (int i = 0; i < 4; i++) {
		if (!neighbors[i])
			continue;
		if (randomized == 0) {
			*neighborIndex = i;
			return neighbors[i];
>>>>>>> fml check what this shit is (NEED IT)
		}
		randomized--;
	}
	return nullptr;
}

void Maze::buildMaze()
{
	stack<Cell*> dfs;
	Cell* neighbors[] = { nullptr, nullptr, nullptr, nullptr };

	/* add start cell for dfs to build the maze from */
	dfs.push(starts[0]);
	starts[0]->setVisited(true);

	int prevIndex;
	int neighborIndex = -1;
	int numNeighbors;
	while (!dfs.empty()) {
		/* get last Cell in dps branching*/
		Cell* cur = dfs.top();
		dfs.pop();

		/* search for his neighbors */
		getNeighbors(cur, neighbors, &numNeighbors);
		/* skip to next iteration if no neighbors */
		if (numNeighbors == 0)
			continue;

		/* continue factor to set same direction */
		prevIndex = neighborIndex;
		Cell* neighbor = getPrevNeighborDirection(neighbors, prevIndex);
		/* if didnt take prev neighbor dirction randomize onee*/
		if (neighbor == nullptr)
			neighbor = randomizeNeighbor(neighbors, numNeighbors, &neighborIndex);
	
		if (!neighbor)
			cout << "something went wrong neighbor is nullptr" << endl;

		/* we would want to iterate back to other neighbors path if there is more than one */
		if (numNeighbors > 1)
			dfs.push(cur);

		/* remove the wall between current cell and the neighbor*/
		removeWall(cur, neighbor, neighborIndex);
		neighbor->setVisited(true);
		dfs.push(neighbor);
	}

	for (unsigned int i = 0; i < cells.size(); i++)
		for (unsigned int j = 0; j < cells[0].size(); j++)
			cells[i][j].setVisited(false);

	/* uncomment for debug purposes */
	/*for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			cout << cells[i][j];*/

}

Maze::Maze(int numRows, int numCols, bool setStartTarget)
{
	allocateMaze(numRows, numCols);	
	if (setStartTarget) {
		addStart(numRows / 2, numCols / 2);
<<<<<<< HEAD
		setTarget((rand() % (numRows - 2)) + 1, (rand() % (numCols - 2)) + 1);
=======
		setTarget(rand() % numRows, rand() % numCols);
		buildMaze();
>>>>>>> fml check what this shit is (NEED IT)
	}
}

bool Maze::addStart(int r, int c)
{
	for (auto& e : starts)
		if (e->equal(r, c))
			return false;

	starts.push_back(&cells[r][c]);
	return true;
}

bool Maze::removeStart(int r, int c)
{
	for (unsigned int i = 0; i < starts.size(); i++) {
		if (starts[i]->equal(r, c)) {
			starts.erase(starts.begin() + i);
			return true;
		}
	}		
	return false;
}

void Maze::drawMaze()
{
	double x, y;
	int numRows = (int)cells.size();
	int numCols = (int)cells[0].size();
	double sx = 2.0 / numRows;
	double sy = 2.0 / numCols;

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			cells[j][i].setOpenGLColor();
			// draw sqaure cell
			x = 2 * (j / (double)numRows) - 1;
			y = 2 * (i / (double)numCols) - 1;
			drawCell(x, y, sx, sy);
		}
	}

	for (unsigned int i = 0; i < starts.size(); i++) {
		x = 2 * (starts[i]->getRow() / (double)numRows) - 1;
		y = 2 * (starts[i]->getCol() / (double)numCols) - 1;
		Cell::setOpenGLStartColor();
		drawCell(x, y, sx, sy);
	}

	x = 2 * (target->getRow() / (double)numRows) - 1;
	y = 2 * (target->getCol() / (double)numCols) - 1;
	Cell::setOpenGLTargetColor();
	drawCell(x, y, sx, sy);

}