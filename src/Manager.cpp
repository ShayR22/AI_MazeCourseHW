#include "Manager.h"
#include "OpenGL.h"
#include "Maze.h"
#include "Graph.h"
#include "MazeSolverAStar.h"
#include "BFSSolver.h"
#include "BFSBiDirectionalSolver.h"
#include "GraphSolverAStar.h"
#include "GraphSolverBestFirstSearch.h"

using namespace std;

void Manager::initAll()
{

	cout << "entered: "<< __func__ << endl;
	if (drawType == DrawableType::MAZE_GAME) {
		mazeGame = new MazeGame(size);
		drawable = mazeGame;
		solvable = mazeGame;
		return;
	}

	Maze* maze = new Maze(size, size);

	if (drawType != DrawableType::MAZE) {
		Graph* g = new Graph(*maze);
		drawable = g;
		switch (solverType) {
		case SolveableType::GRAPH_A_STAR:
			solvable = new GraphSolverAStar(*g);
			break;
		case SolveableType::GRAPH_BEST_FIRST_SEARCH:
			solvable = new GraphSolverBestFirstSearch(*g);
			break;
		}

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
	case SolveableType::MAZE_A_STAR:
		solvable = new MazeSolverAStar(*maze);
		break;
	}
}

void Manager::destroyAll()
{
	if (mazeGame) {
		delete mazeGame;
	} else {
		delete drawable;
		delete solvable;
	}

	drawable = nullptr;
	solvable = nullptr;
	mazeGame = nullptr;
}

bool Manager::isNewGame(DrawableType drawType, SolveableType solverType)
{
	if ((drawType == DrawableType::MAZE_GAME && solverType != SolveableType::MAZE_GAME) ||
		(drawType != DrawableType::MAZE_GAME && solverType == SolveableType::MAZE_GAME))
		throw "when maze game is seleceted user must set both draw and solve type to MAZE_GAME";

	return drawType == DrawableType::MAZE_GAME && solverType == SolveableType::MAZE_GAME;
}

void Manager::verifyDrawableSolveable(DrawableType drawType, SolveableType solverType)
{
	if (solverType == SolveableType::NONE)
		return;

	if (isNewGame(drawType, solverType))
		return;

	if (drawType == DrawableType::GRAPH) {
		switch (solverType) {
		case SolveableType::GRAPH_A_STAR:
			break;
		case SolveableType::GRAPH_BEST_FIRST_SEARCH:
			break;
		default:
			throw runtime_error("Invalid usage, Graph cant use solverType given");
		}
		return;
	}
	
	switch (solverType) {
	case SolveableType::MAZE_BFS:
		break;
	case SolveableType::MAZE_BFS_BI:
		break;
	case SolveableType::MAZE_A_STAR:
		break;
	default:
		throw runtime_error("Invalid usage, Mazes cant use solverType given");
	}
}

void Manager::draw() 
{
	if (drawable)
		drawable->draw();
}

void Manager::solveIteration()
{
	if (solvable)
		solvable->solveIteration();
}

Manager::Manager(unsigned int mazeSize, DrawableType drawType, SolveableType solverType) :
	drawable(nullptr), solvable(nullptr), size(mazeSize), drawType(drawType), solverType(solverType),
	mazeGame(nullptr)
{
	solved = false;
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
