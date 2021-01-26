#include "MazeMovingObj.h"
#include "MovingObj.h"
#include "OpenGL.h"
#include <iostream>

MazeMovingObj::MazeMovingObj(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY)
	: MovingObj(x, y, maxDx, maxDy, targetX, targetY), cells(cells), lastCellX((int)x), lastCellY((int)y) {}


void MazeMovingObj::getOpenGLCellCenter(Cell& cell, float* xy)
{
	float newX = cell.getX() + 0.5f;
	float newY = cell.getY() + 0.5f;

	xy[0] = newX;
	xy[1] = newY;
}

void MazeMovingObj::setTarget(Cell& cell)
{
	float xy[2];
	getOpenGLCellCenter(cell, xy);
	MovingObj::setTarget(xy[0], xy[1]);
}

void MazeMovingObj::setLocation(Cell& cell)
{
	float xy[2];
	getOpenGLCellCenter(cell, xy);
	MovingObj::setLocation(xy[0], xy[1]);
}


void MazeMovingObj::updateLastLocation()
{
	if ((int)x == lastCellX && (int)y == lastCellY)
		return;

	lastCellX = (int)x;
	lastCellY = (int)y;
}