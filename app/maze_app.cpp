#include <time.h>
#include "glut.h"
#include "OpenGL.h"
#include "Manager.h"
constexpr int MSIZE = 25;

Manager manager(MSIZE);

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	manager.draw();
	glutSwapBuffers(); // show all
}

void idle()
{
	manager.solveIteration();
	glutPostRedisplay(); // indirect call to display
}


void menu(int choice)
{
	switch (choice)
	{
	case 0: // Restart
		manager.restart();
		break;
	case 1: // Graph
		manager.setDrawableSolvable(DrawableType::GRAPH, SolveableType::NONE);
		break;
	case 2: // Maze
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::NONE);
		break;
	case 3: // MAZE BFS
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::MAZE_BFS);
		break;
	case 4: // MAZE BFS BI
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::MAZE_BFS_BI);
		break;
	case 5: // MAZE A STAR
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::MAZE_A_STAR);
		break;
	case 6: // GRAPH A STAR
		manager.setDrawableSolvable(DrawableType::GRAPH, SolveableType::GRAPH_A_STAR);
		break;
	case 7: // GRAPH Best First Search
		manager.setDrawableSolvable(DrawableType::GRAPH, SolveableType::GRAPH_BEST_FIRST_SEARCH);
		break;
	case 8: // MAZE GAME
		manager.setDrawableSolvable(DrawableType::MAZE_GAME, SolveableType::MAZE_GAME);
		break;
	}
}

void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, 0.0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand((unsigned int)time(0));

	OpenGL::width = MSIZE;
	OpenGL::height = MSIZE;
	OpenGL::circleR = 1 / (float)MSIZE;
	OpenGL::cubeW = 2 / (float)MSIZE;
	OpenGL::cubeH = 2 / (float)MSIZE;

	//manager.restart();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(400, 0);
	glutCreateWindow("First Example");

	/* make drawing points to draw circle instead of pixel */
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("Graph", 1);
	glutAddMenuEntry("Maze", 2);
	glutAddMenuEntry("MAZE_BFS", 3);
	glutAddMenuEntry("MAZE_BFS BI", 4);
	glutAddMenuEntry("MAZE_A_STAR", 5);
	glutAddMenuEntry("GRAPH_A_STAR", 6);
	glutAddMenuEntry("GRAPH_BEST_FIRST_SEARCH", 7);
	glutAddMenuEntry("MAZE_GAME", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutMainLoop();

	return 0;
}