#include <glut.h>
#include "OpenGL.h"
#include "Node.h"
#include "Edge.h"

using namespace std;

Node::Node(int x, int y) : x(x), y(y), heuristic(0), visited(false) {}

Node::Node(const Node& node) : x(node.x), y(node.y), edges(node.edges), heuristic(node.heuristic), visited(node.visited) {}

void Node::addEdge(Edge& edge)
{
	edges.push_back(&edge);
}

vector<Node*> Node::getNieghbors()
{
    vector<Node*> neighbors;

    for (unsigned int i = 0; i < edges.size(); i++)
        neighbors.push_back(edges[i]->getTo());

    return neighbors;
}

void Node::draw()
{
    float w = OpenGL::cubeW / 2;
    float h = OpenGL::cubeH / 2;
    /* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
    /* y might not need cuz we draw in downward direction */
    float ax = (2 * ((float)x+ 0.5f ) / OpenGL::width) - 1;
    float ay = (2 * (float)y / OpenGL::height) - 1;
 

    glBegin(GL_POLYGON);  // fill up
    glVertex2f(ax, ay);
    glVertex2f(ax, ay + h);
    glVertex2f(ax + w, ay + h);
    glVertex2f(ax + w, ay);
    glEnd();
}

void Node::drawPoints(std::vector<Node*>& nodes)
{
    float w = OpenGL::cubeW / 2;
    float h = OpenGL::cubeH / 2;
    glColor3d(0, 0, 1);
    for (unsigned int i = 0; i < nodes.size(); i++) {
        /* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
        /* y might not need cuz we draw in downward direction */
        float x = (2 * ((float)nodes[i]->getX() + 0.5f) / OpenGL::width) - 1;
        float y = (2 * (float)nodes[i]->getY() / OpenGL::height) - 1;
       

        glBegin(GL_POLYGON);  // fill up
        glVertex2f(x, y);
        glVertex2f(x, y + h);
        glVertex2f(x + w, y + h);
        glVertex2f(x + w, y);
        glEnd();
    }
    glColor3d(0, 0, 0);

}

void Node::setHeuristic(double h) 
{
    heuristic = h;
}



