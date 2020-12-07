#include <math.h>
#include "glut.h"
#include "OpenGL.h"
#include "Edge.h"
#include "Node.h"


double Edge::calcWeight()
{
	int x1 = from->getX();
	int y1 = from->getY();
	int x2 = to->getX();
	int y2 = to->getY();

	if (x1 == x2)
		return fabs((double)y2 - y1);
	if (y1 == y2)
		return fabs((double)x2 - x1);
	
	return sqrt(pow((double)x2 - x1, 2) + pow((double)y2 - y1, 2));
}

Edge::Edge(Node* from, Node* to) : from(from), to(to), weight(calcWeight()) {}

void Edge::draw()
{
	float w = OpenGL::cubeW / 4;
	float h = OpenGL::cubeH / 4;
	/* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
	/* y might not need cuz we draw in downward direction */
	float x1 = 2 * (((float)from->getX() + 0.5f) / OpenGL::width) - 1 + w;
	float x2 = 2 * (((float)to->getX() + 0.5f) / OpenGL::width) - 1 + w;
	float y1 = 2 * ((float)from->getY() / OpenGL::height) - 1 + h;
	float y2 = 2 * ((float)to->getY() / OpenGL::height) - 1 + h;

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void Edge::drawEdges(std::vector<Edge*>& edges)
{
	float w = OpenGL::cubeW / 4;
	float h = OpenGL::cubeH / 4;

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (unsigned int i = 0; i < edges.size(); i++) {
		Node* from = edges[i]->getFrom();
		Node* to = edges[i]->getTo();

		/* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
		/* y might not need cuz we draw in downward direction */
		float y1 = 2 * (((float)from->getX() + 0.5f) / OpenGL::width) - 1 + w;
		float y2 = 2 * (((float)to->getX() + 0.5f) / OpenGL::width) - 1 + w;
		float x1 = 2 * ((float)from->getY() / OpenGL::height) - 1 + h;
		float x2 = 2 * ((float)to->getY() / OpenGL::height) - 1 + h;

		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}

	glEnd();
}