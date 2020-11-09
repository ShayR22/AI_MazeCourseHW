#include <time.h>
#include "glut.h"
#include "OpenGL.h"
#include "Maze.h"
#include "BFSSolver.h"
#include "BFSBiDirectionalSolver.h"
#include "Graph.h"
constexpr int MSIZE = 100;

Maze* maze = nullptr;
BFSSolver* bfs_solver = nullptr;
BFSBiDirectionalSolver* bi_bfs_solver = nullptr;
Graph* graph = nullptr;
bool bfs_is_on = false;
bool bi_bfs_is_on = false;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	graph->draw();
	glutSwapBuffers(); // show all
}

void idle()
{
	if (bfs_is_on)
		bfs_solver->solveIteration();
	else if (bi_bfs_is_on)
		bi_bfs_solver->solveIteration();

	glutPostRedisplay(); // indirect call to display
}

void initGlobals()
{
	maze = new Maze(MSIZE, MSIZE);
	bfs_solver = new BFSSolver(*maze);
	bi_bfs_solver = new BFSBiDirectionalSolver(*maze);
	graph = new Graph(*maze);
}

void setSolverFalse()
{
	bfs_is_on = false;
	bi_bfs_is_on = false;
}

void destroyGlobals()
{
	delete maze;
	delete bfs_solver;
	delete bi_bfs_solver;
	delete graph;
}

void restart()
{
	setSolverFalse();
	destroyGlobals();
	initGlobals();
}

void menu(int choice)
{
	switch (choice)
	{
	case 0: // Restart
		restart();
		break;
	case 1: // BFS
		bfs_is_on = true;
		break;
	case 2: // BFS Bi
		bi_bfs_is_on = true;
		break;
	}
}

void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, 0.0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand((unsigned int)time(0));

	restart();

	OpenGL::width = MSIZE;
	OpenGL::height = MSIZE;
	OpenGL::circleR = 1 / (float)MSIZE;
	OpenGL::cubeW = 2 / (float)MSIZE;
	OpenGL::cubeH = 2 / (float)MSIZE;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(400, 0);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("BFS BI", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutMainLoop();
	destroyGlobals();

	return 0;
}