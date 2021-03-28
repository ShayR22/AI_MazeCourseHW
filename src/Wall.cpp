#include "Wall.hpp"
#include "Obstacle.hpp"
#include "Drawer.hpp"




Wall::Wall(int healthPoints, int destroyFrame, std::vector<Cell*> cover) : Obstacle(healthPoints, destroyFrame, cover)
{
	
}


void Wall::draw(float offsetX, float offsetY)
{
	for (auto& c : cover) {
		float x = offsetX + c->getX();
		float y = offsetY + c->getY();
		Drawer::rect(x, y, 1.0f, 1.0f, DrawerColor::BLACK);
	}
}


bool Wall::collision(MovingObject& movingObject)
{
	return true;
}
