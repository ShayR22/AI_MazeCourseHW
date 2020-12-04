#include "Manager.h"
#include "Maze.h"
#include "Graph.h"
#include "BFSSolver.h"
#include "BFSBiDirectionalSolver.h"

void Manager::initAll()
{
	solvable = nullptr;
	drawable = nullptr;
	Maze* maze = new Maze(size, size);

	if (drawType != DrawableType::MAZE) {
		drawable = new Graph(*maze);
		delete maze;
		return;
	}
	
	drawable = maze;
	switch (solverType) {
	case SolveableType::MAZE_BFS:
		solvable = new BFSSolver(*maze);
		break;
	case SolveableType::MAZE_BFS_BI:
		solvable = new BFSBiDirectionalSolver(*maze);
		break;
	}
}

void Manager::destroyAll()
{
	delete drawable;
	delete solvable;
}

void Manager::verifyDrawableSolveable(DrawableType drawType, SolveableType solverType)
{
	/* currently graph have no solvers */
	if (drawType != DrawableType::MAZE)
		if (solverType != SolveableType::NONE)
			throw "Invalid usage of DrawableType and SolvableTypes";

}


Manager::Manager(unsigned int mazeSize, DrawableType drawType, SolveableType solverType) :
	drawable(nullptr), solvable(nullptr), size(mazeSize), drawType(drawType), solverType(solverType)
{
	verifyDrawableSolveable(drawType, solverType);
	restart();
}

Manager::~Manager()
{
	destroyAll();
}

void Manager::restart()
{
	destroyAll();
	initAll();
}

void Manager::setSize(unsigned int newSize)
{
	size = newSize;
	restart();
}

void Manager::setDrawableSolvable(DrawableType drawableType, SolveableType solvableType)
{
	verifyDrawableSolveable(drawableType, solvableType);
	drawType = drawableType;
	solverType = solvableType;
	restart();
}
