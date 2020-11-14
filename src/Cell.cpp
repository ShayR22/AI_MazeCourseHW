#include <iostream>
#include "glut.h"
#include "Cell.h"

using namespace std;

Cell::Cell() :
	row(0), col(0), state(CellState::wall), 
	visiting(false), visited(false)
{
	memset(walls, true, sizeof(walls));
}

Cell::Cell(int row, int col, const CellState& state) :
	row(row), col(col), state(state), 
	visiting(false), visited(false)
{
	memset(walls, true, sizeof(walls));
}

void Cell::setLocation(int r, int c)
{
	row = r;
	col = c;
}

void Cell::setOpenGLColor()
{
	if (state == CellState::path) {
		glColor3d(0.8, 0.4, 1);   // magenta
		return;
	}

	if (visiting) { // gray cell
		glColor3d(1, 1, 0);   // yellow
		return;
	}
	if (visited) { // black cell
		glColor3d(0.8, 1, 0.8);   // green
		return;
	}

	switch (state) {
	case CellState::space:
		glColor3d(1, 1, 1);   // white
		break;
	case CellState::wall:
		glColor3d(0.7, 0.4, 0);   // dark-orange
		break;
	default:
		cout << "invalid state, something went wrong" << endl;
		break;
	}
}

void Cell::draw()
{
	float x = 2 * (float)col / OpenGL::width - 1;
	float y = 2 *(float)row / OpenGL::height - 1;
	float w = OpenGL::cubeW;
	float h = OpenGL::cubeH;

	glBegin(GL_POLYGON);  // fill up
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
	glColor3d(0, 0, 0);
	/* walls: left, top, right, down */
	float xy[5][2] = {
		{x, y + h}, /* down left*/
		{x, y}, /* top left */
		{x + w, y}, /* top right */
		{x + w, y + h}, /* down right*/
		{x, y + h} /* down left (this makes loop easier)*/
	};

	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++) {
		if (!walls[i])
			continue;

		glVertex2f(xy[i][0], xy[i][1]);
		glVertex2f(xy[i+1][0], xy[i+1][1]);
	}
	glEnd();

}

ostream& operator<<(ostream& os, const Cell& c)
{
	os << "cellType: " << (int)c.state;
	os << "\trow: " << c.row << "\tcol : " << c.col;
	os << "\tvisiting: " << c.visiting << "\tvisited: " << c.visited;
	os << endl;
	return os;
}
