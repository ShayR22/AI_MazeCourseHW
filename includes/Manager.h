#ifndef __MANAGER__
#define __MANAGER__

#include "Player.h"
#include "Drawable.h"
#include "Solvable.h"
#include "Maze.h"
#include "MazeGame.h"


enum class SolveableType {
	MAZE_BFS,
	MAZE_BFS_BI,
	MAZE_A_STAR,
	MAZE_GAME,
	GRAPH_A_STAR,
	GRAPH_BEST_FIRST_SEARCH,
	NONE,
};

enum class DrawableType {
	MAZE,
	MAZE_GAME,
	GRAPH,
};

/* FIXME change this to a better class name */
class Manager : public Drawable, public Solveable {
private:
	unsigned int size;
	SolveableType solverType;
	DrawableType drawType;
	Drawable *drawable;
	Solveable* solvable;
	MazeGame* mazeGame;
	
	bool isNewGame(DrawableType drawType, SolveableType solverType);
	void initAll();
	void destroyAll();
	void verifyDrawableSolveable(DrawableType drawType, SolveableType solverType) noexcept(false);
public:
	Manager(unsigned int mazeSize, DrawableType drawType= DrawableType::MAZE,
					SolveableType solverType = SolveableType::NONE) noexcept(false);
	~Manager();
	void restart();
	void setSize(unsigned int newSize);
	void setDrawableSolvable(DrawableType drawableType, SolveableType solvableType) noexcept(false);

	virtual void draw() override;
	virtual void solveIteration() override;
};



#endif 