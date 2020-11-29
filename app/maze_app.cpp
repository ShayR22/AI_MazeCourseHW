#include <time.h>
#include "glut.h"
#include "OpenGL.h"
#include "Manager.h"
constexpr int MSIZE = 100;

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
	case 3: // BFS
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::MAZE_BFS);
		break;
	case 4: // BFS BI
		manager.setDrawableSolvable(DrawableType::MAZE, SolveableType::MAZE_BFS_BI);
		break;
	}
}

void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, 0.0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand((unsigned int)time(0));

	manager.restart();

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
	glutAddMenuEntry("Graph", 1);
	glutAddMenuEntry("Maze", 2);
	glutAddMenuEntry("MAZE_BFS", 3);
	glutAddMenuEntry("MAZE_BFS BI", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutMainLoop();

	return 0;
}