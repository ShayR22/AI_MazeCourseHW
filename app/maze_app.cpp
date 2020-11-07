#include <time.h>
#include "glut.h"
#include "Maze.h"
#include "BFSSolver.h"
#include "BFSBiDirectionalSolver.h"

constexpr int MSIZE = 25;

Maze* maze = nullptr;
BFSSolver* bfs_solver = nullptr;
BFSBiDirectionalSolver* bi_bfs_solver = nullptr;
bool bfs_is_on = false;
bool bi_bfs_is_on = false;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	maze->drawMaze();
	glutSwapBuffers(); // show all
}



void idle()
{
	if (bfs_is_on) {

		bfs_solver->solveIteration();
	}

	else if (bi_bfs_is_on)
	{
		bi_bfs_solver->solveIteration();
	}

	glutPostRedisplay(); // indirect call to display
}

void restart()
{
	delete maze;
	delete bfs_solver;
	delete bi_bfs_solver;
	bfs_is_on = false;
	bi_bfs_is_on = false;
	maze = new Maze(MSIZE, MSIZE);
	bfs_solver = new BFSSolver(*maze);
	bi_bfs_solver = new BFSBiDirectionalSolver(*maze);
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
	case 2: // DFS
		break;
	case 3: // BFS Bi
		bi_bfs_is_on = true;
		break;
	}
}

void init()
{
	glClearColor(0.8, 0.7, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand(time(0));
	restart();
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("First Example");
	//glEnable(GL_COLOR_MATERIAL);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("DFS", 2);
	glutAddMenuEntry("BFS BI", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutMainLoop();

	std::cout << "going to delete" << std::endl;

	delete maze;
	delete bfs_solver;
	delete bi_bfs_solver;
}