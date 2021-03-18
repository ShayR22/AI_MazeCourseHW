#include "Room.hpp"
#include "Corridor.hpp"
#include "Drawer.hpp"

using namespace std;

Room::Room(CellMat& cells, vec2f& xyOffset) : BoardCells(cells, true, xyOffset)
{
	/* DO NOTHING */
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

void Room::draw()
{
	/* Assume cells aren't empty */
	float w = static_cast<float>(cells[0].size());
	float h = static_cast<float>(cells.size());

	Drawer::rectWithGrid(xyOffset.x, xyOffset.y, w, h, DrawerColor::WHITE);
}