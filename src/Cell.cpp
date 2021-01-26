#include <iostream>
#include "glut.h"
#include "OpenGL.h"
#include "Cell.h"

using namespace std;
using namespace std;

Cell::Cell() :
	row(0), col(0), isPath(false), hasCoin(false),
	visiting(false), visited(false), parent(nullptr)
{
	memset(walls, true, sizeof(walls));
}

Cell::Cell(int row, int col) :
	row(row), col(col), isPath(false), hasCoin(false),
	visiting(false), visited(false), parent(nullptr)
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
	if (isPath) {
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

	// regular
	glColor3d(1, 1, 1);   // white
}


void Cell::drawPoly(float x, float y, float w, float h)
{
	glBegin(GL_POLYGON);  // fill up
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}


void Cell::drawCoin(float x, float y, float w, float h)
{
	if (!hasCoin)
		return;

	glPointSize(1 / OpenGL::circleR / 4.0f);
	glBegin(GL_POINTS);
	/* gold color is 255, 215, 0 (255/255, 215/255, 0/255) */
	glColor3d(1.0, 0.84, 0);
	glVertex2f(x + w / 2.0f, y + h / 2.0f);
	glEnd();
}

void Cell::drawWalls(int numWalls, float x, float y, float w, float h)
{
	/* walls: left, top, right, down */
	float xy[5][2] = {
		{x, y + h}, /* down left*/
		{x, y}, /* top left */
		{x + w, y}, /* top right */
		{x + w, y + h}, /* down right*/
		{x, y + h} /* down left (this makes loop easier)*/
	};

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < numWalls; i++) {
		if (!walls[i])
			continue;

		glVertex2f(xy[i][0], xy[i][1]);
		glVertex2f(xy[i + 1][0], xy[i + 1][1]);
	}
	glEnd();
}

void Cell::draw()
{
	float x = 2 * (float)col / OpenGL::width - 1;
	float y = -(2 *(float)row / OpenGL::height - 1);
	float w = OpenGL::cubeW;
	float h = -OpenGL::cubeH;

	drawPoly(x, y, w, h);
	drawWalls(sizeof(walls), x, y, w, h);
	drawCoin(x, y, w, h);
}

/* smart draw for preformance for the maze usage*/
void Cell::drawTopLeft()
{
	float x = 2 * (float)col / OpenGL::width - 1;
	float y = -(2 * (float)row / OpenGL::height - 1);
	float w = OpenGL::cubeW;
	float h = - OpenGL::cubeH;

	drawPoly(x, y, w, h);
	/* draw only top and left walls by using 2 walls*/
	drawWalls(2, x, y, w, h);
	drawCoin(x, y, w, h);
}

ostream& operator<<(ostream& os, const Cell& c)
{
	os << "cellType: " << (int)c.isPath;
	os << "\trow: " << c.row << "\tcol : " << c.col;
	os << "\tvisiting: " << c.visiting << "\tvisited: " << c.visited;
	os << endl;
	return os;
}
