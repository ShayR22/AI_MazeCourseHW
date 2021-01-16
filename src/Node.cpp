#include <glut.h>
#include "OpenGL.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>

using namespace std;

Node::Node(int x, int y) : x(x), y(y), visited(false), isVisiting(false), isPath(false), parent(nullptr) {}

Node::Node(const Node& node) : x(node.x), y(node.y), edges(node.edges), visited(node.visited),
isVisiting(node.isVisiting), isPath(node.isPath), parent(node.parent) {}

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
    float h = -OpenGL::cubeH / 2;
    /* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
    /* y might not need cuz we draw in downward direction */
    float ay = -((2 * ((float)x) / OpenGL::width) - 1) + h/2.0f;
    float ax = (2 * (float)y / OpenGL::height) - 1 + w/2.0f;
 
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
    float h = -OpenGL::cubeH / 2;
    glColor3d(0, 0, 1);

    for (unsigned int i = 0; i < nodes.size(); i++) {

        if(nodes[i]->getIsPath())
            glColor3d(0.8, 0.4, 1);
        else if(nodes[i]->getIsVisiting())
            glColor3d(1, 1, 0);
        else if(nodes[i]->getVisited())
            glColor3d(0.8, 1, 0.8);
        else
            glColor3d(0, 0, 1);

        /* + 0.5 to compensate for matrix to go (0:size-1) not sure why not needed in y */
        /* y might not need cuz we draw in downward direction */
        float y = -((2 * ((float)nodes[i]->getX()) / OpenGL::width) - 1) + h / 2.0f;
        float x = (2 * (float)nodes[i]->getY() / OpenGL::height) - 1  + w / 2.0f;
       

        glBegin(GL_POLYGON);  // fill up
        glVertex2f(x, y);
        glVertex2f(x, y + h);
        glVertex2f(x + w, y + h);
        glVertex2f(x + w, y);
        glEnd();
    }
    glColor3d(0, 0, 0);

}




