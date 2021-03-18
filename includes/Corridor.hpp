#ifndef __CORRIDOR__
#define __CORRIDOR__

#include "Cell.hpp"
#include "BoardCells.hpp"
#include "Consumable.hpp"
#include "vec2.h"
#include <map>
#include <vector>

class Room;

class Corridor : public BoardCells {
private:
	std::map<Cell*, Room*> roomConnections;
public:
	Corridor(CellMat& cells, vec2f& xyOffset);
	Corridor(std::map<Cell*, Room*> roomConnections, CellMat& cells, vec2f& xyOffset);

	virtual Cell* getConnectingCell(BoardCells& board);
	virtual std::vector<BoardCells*> getConnectors();
	virtual std::vector<Consumable*> getConsumables(ConsumableType type);
	void draw();

	inline void setRoomConnections(std::map<Cell*, Room*> roomConnections) { this->roomConnections = roomConnections; }
	inline std::map<Cell*, Room*>& getRoomConnections() { return this->roomConnections; }
};

#endif