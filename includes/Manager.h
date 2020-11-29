#ifndef __MANAGER__
#define __MANAGER__

#include "Drawable.h"
#include "Solvable.h"

enum class SolveableType {
	MAZE_BFS,
	MAZE_BFS_BI,
	NONE,
};

enum class DrawableType {
	MAZE,
	GRAPH
};

/* FIXME change this to a better class name */
class Manager : public Drawable, Solveable {
private:
	unsigned int size;
	SolveableType solverType;
	DrawableType drawType;
	Drawable *drawable;
	Solveable* solvable;
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

	virtual void draw() override
	{
		if (drawable)
			drawable->draw();
	}
	virtual void solveIteration() override
	{
		if (solvable)
			solvable->solveIteration();
	}
};



#endif 