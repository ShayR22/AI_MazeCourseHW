#include <iostream>
#include "glut.h"
#include "Cell.h"

using namespace std;

Cell::Cell() : 
	row(0), col(0), state(CellState::wall), 
	visiting(false), visited(false) {}

Cell::Cell(int row, int col, const CellState& state) :
	row(row), col(col), state(state), 
	visiting(false), visited(false) {}

void Cell::setLocation(int r, int c)
{
	row = r;
	col = c;
}

void Cell::setOpenGLColor()
{
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

ostream& operator<<(ostream& os, const Cell& c)
{
	os << "cellType: " << (int)c.state;
	os << "\trow: " << c.row << "\tcol : " << c.col;
	os << "\tvisiting: " << c.visiting << "\tvisited: " << c.visited;
	os << endl;
	return os;
}
