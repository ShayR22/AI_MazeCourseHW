#ifndef __CORRIDOR__
#define __CORRIDOR__

#include "Cell.hpp"
#include "Room.hpp"
#include "Consumable.hpp"
#include <map>
#include <vector>

class Corridor : public BoardCells {
private:
	std::map<Cell*, Room*> roomConnections;
	std::vector<std::vector<Cell>> cells;
public:
	Corridor(std::map<Cell*, Room*>  roomConnections, std::vector<std::vector<Cell>> cells);

	virtual Cell* getConnectingCell(BoardCells& board);
	virtual std::vector<std::vector<Cell>>& getCells();
	virtual std::vector<BoardCells*> getConnectors();
	virtual std::vector<Consumable>& getConsumables(enum consumableType);
	void draw();
};

#endif