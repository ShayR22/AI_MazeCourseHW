#include "Enemy.h"
#include "OpenGL.h"
#include <iostream>


Enemy::Enemy(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int brainCalculationEachMs)
    : MazeMovingObj(cells, x, y, maxDx, maxDy, targetX, targetY), brainCalculationEachMs(brainCalculationEachMs), lastTick(std::chrono::high_resolution_clock::now())
{
    lastCellX = (int)x;
    lastCellY = (int)y;

    if (brainCalculationEachMs <= 0) {
        std::cout << "warning: negative value given for brainCalculationEachMs" << std::endl;
        std::cout << "setting to " << MIN_BRAIN_CALC_TIMEOUT_MS << std::endl;
        this->brainCalculationEachMs = MIN_BRAIN_CALC_TIMEOUT_MS;
    }
}

void Enemy::draw()
{
    float y = -(2 * (float)this->x / OpenGL::width - 1);
    float x = (2 * (float)this->y / OpenGL::height - 1);

    /* draw black body and draw body yellow a bit smaller, in order to create a black
     * layer creating clear definition for packman
     */
    glPointSize(1 / OpenGL::circleR / 1.2f);
    glBegin(GL_POINTS);
    glColor3d(0, 0, 0);
    glVertex2f(x, y);
    glEnd();

    glPointSize(1 / OpenGL::circleR / 1.5f);
    glBegin(GL_POINTS);
    glColor3d(1, 0, 0);
    glVertex2f(x, y);
    glEnd();

    /* put location of the eye in the direction of movement*/
    y -= OpenGL::circleR * (dx / maxDx) / 4.5f;
    x += OpenGL::circleR * (dy / maxDy) / 4.5f;

    /* draw sort of an eye */
    glPointSize(1 / OpenGL::circleR / 4.0f);
    glBegin(GL_POINTS);
    glColor3d(0, 0, 0);
    glVertex2f(x, y);
    glEnd();
}

void Enemy::move()
{
    MovingObj::move();
    if ((int)x == lastCellX && (int)y == lastCellY)
        return;

    lastCellX = (int)x;
    lastCellY = (int)y;
}