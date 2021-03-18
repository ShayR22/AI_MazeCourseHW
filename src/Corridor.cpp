#include "Corridor.hpp"
#include "Room.hpp"
#include "Drawer.hpp"

using namespace std;

Corridor::Corridor(CellMat& cells, vec2f& xyOffset) : BoardCells(cells, false, xyOffset)
{
	/* DO NOTHING */
}

Corridor::Corridor(map<Cell*, Room*> roomConnections, CellMat& cells, vec2f& xyOffset) :
	BoardCells(cells, false, xyOffset), roomConnections(roomConnections)
{
	/* DO NOTHING */
}


Cell* Corridor::getConnectingCell(BoardCells& board)
{
	for (auto& e : roomConnections) {
		if (&board == static_cast<BoardCells*>(e.second))
			return e.first;
	}

	return nullptr;
}

vector<BoardCells*> Corridor::getConnectors()
{
	vector<BoardCells*> connectors(roomConnections.size());

	for (auto& e : roomConnections)
		connectors.push_back(static_cast<BoardCells*>(e.second));

	return connectors;

}

vector<Consumable*> Corridor::getConsumables(ConsumableType type)
{
	/* Currently Corridors doesn't hold consumables */
	vector<Consumable*> empty;
	return empty;
}

void Corridor::draw()
{
	/* Assume cells aren't empty */
	float w = static_cast<float>(cells[0].size());
	float h = static_cast<float>(cells.size());

	Drawer::rectWithGrid(xyOffset.x, xyOffset.y, w, h, DrawerColor::WHITE);
}

