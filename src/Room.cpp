#include "Room.hpp"
#include "Corridor.hpp"
#include "AmmoBox.hpp"
#include "HealthBox.hpp"
#include "Wall.hpp"
#include "Drawer.hpp"

using namespace std;

Room::Room(CellMat& cells, vec2f& xyOffset) : BoardCells(cells, true, xyOffset)
{

}

Room::Room(map<Cell*, Corridor*> corridorConnections, CellMat& cells,
	vector<Obstacle*> obstacles, vector<Consumable*> ammoBoxes,
	vector<Consumable*> healthBoxes, vec2f &xyOffset) :
	BoardCells(cells, true, xyOffset), corridorConnections(corridorConnections), obstacles(obstacles),
	ammoBoxes(ammoBoxes), healthBoxes(healthBoxes)
{
	/* DO NOTHING */
}

Cell* Room::getConnectingCell(BoardCells& board)
{
	for (auto& e : corridorConnections) {
		if (&board == static_cast<BoardCells*>(e.second))
			return e.first;
	}

	return nullptr;
}

vector<BoardCells*> Room::getConnectors()
{
	vector<BoardCells*> connectors(corridorConnections.size());

	for (auto& e : corridorConnections)
		connectors.push_back(static_cast<BoardCells*>(e.second));

	return connectors;
}

vector<Consumable*> Room::getConsumables(ConsumableType type)
{
	if (type == ConsumableType::AMMO)
		return ammoBoxes;
	else
		return healthBoxes;
}

void Room::addAmmoBox(int cellX, int cellY, int ammoAmount, bool isHidden)
{
	Cell* location = &(cells[cellY][cellX]);
	location->setIsOccupy(true);
	Consumable* ammoBox = new AmmoBox(location, ammoAmount, isHidden);
	ammoBoxes.push_back(ammoBox);
}


void Room::addHealthBox(int cellX, int cellY, int healthAmount, bool isHidden)
{

	Cell* location = &(cells[cellY][cellX]);
	location->setIsOccupy(true);
	Consumable* healthBox = new HealthBox(location, healthAmount, isHidden);
	healthBoxes.push_back(healthBox);
}

void Room::addWall(int healthPoints, int destroyFrame, vector<Cell*>& cover)
{
	Wall* wall = new Wall(healthPoints, destroyFrame, cover);

	for (auto& c : cover) {
		c->setIsOccupy(true);
	}
	obstacles.push_back(wall);
}


void Room::draw()
{
	/* Assume cells aren't empty */
	float w = static_cast<float>(cells[0].size());
	float h = static_cast<float>(cells.size());

	Drawer::rectWithGrid(xyOffset.x, xyOffset.y, w, h, DrawerColor::WHITE);
	
	for (auto& ammoBox : ammoBoxes) {
		ammoBox->draw(xyOffset.x, xyOffset.y);
	}

	for (auto& healthBox : healthBoxes) {
		healthBox->draw(xyOffset.x, xyOffset.y);
	}


	for (auto& obstacle : obstacles) {
		obstacle->draw(xyOffset.x, xyOffset.y);
	}
}