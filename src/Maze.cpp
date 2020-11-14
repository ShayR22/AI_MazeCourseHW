#include <iostream>
#include "glut.h"
#include "Maze.h"

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
			cur_row.push_back(Cell(i, j, CellState::wall));

		cells.push_back(cur_row);
	}
}

void Maze::buildMaze()
{
	int size = cells.size();
	for (int i = 0; i < size; i++) {
		cells[0][i].setState(CellState::wall);
		cells[size - 1][i].setState(CellState::wall);
		cells[i][0].setState(CellState::wall);
		cells[i][size - 1].setState(CellState::wall);;
	}

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
		}

	/* uncomment for debug purposes */
	/*for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			cout << cells[i][j];*/

}

Maze::Maze(int numRows, int numCols, bool setStartTarget)
{
	allocateMaze(numRows, numCols);	
	buildMaze();
	if (setStartTarget) {
		addStart(numRows / 2, numCols / 2);
		setTarget((rand() % (numRows - 2)) + 1, (rand() % (numCols - 2)) + 1);
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