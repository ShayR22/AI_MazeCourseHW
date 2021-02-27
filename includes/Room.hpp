#ifndef __ROOM__
#define __ROOM__

#include "BoardCells.hpp"
#include "Cell.hpp"
#include "Corridor.hpp"
#include "Obstacle.hpp"
#include "Consumable.hpp"
#include <map>
#include <vector>

class Room : public BoardCells {
private:
	std::map<Cell*, Corridor*> corridorConnections;
	std::vector<std::vector<Cell>> cells;
	std::vector<Corridor*> corridors;
	std::vector<Obstacle> obstacles;
	std::vector<Consumable> ammoBoxes;
	std::vector<Consumable> healthBoxes;
public:
	Room(std::map<Cell*, Corridor*> corridorConnections, std::vector<std::vector<Cell>> cells,
		std::vector<Corrider*> corridors, std::vector<Obstacle> obstacles,
		std::vector<Consumable> ammoBoxes, std::vector<Consumable> healthBoxes);

	virtual Cell* getConnectingCell(BoardCells& board);
	virtual std::vector<std::vector<Cell>>& getCells();
	virtual std::vector<BoardCells*> getConnectors();
	virtual std::vector<Consumable>& getConsumables(enum consumableType);
	void draw();

	inline void setCorridorConnections() { this->corridorConnections = corridorConnections; }
	inline std::map<Cell*, Corridor*>& getCorridorConnections() { return corridorConnections; }

	inline void setCells() { this->cells = cells; }
	inline std::vector<std::vector<Cell>>& getCells() { return cells; }

	inline void setCorridors() { this->corridors = corridors; }
	inline std::vector<Corridor*>& getCorridors() { return corridors; }

	inline void setObstacles() { this->obstacles = obstacles; }
	inline std::vector<Obstacle>& getObstacles() { return obstacles; }

	inline void setAmmoBoxes() { this->ammoBoxes = ammoBoxes; }
	inline std::vector<Consumable>& getAmmoBoxes() { return ammoBoxes; }

	inline void setHealthBoxes() { this->healthBoxes = healthBoxes; }
	inline std::vector<Consumable>& getHealthBoxes() { return healthBoxes; }
};

#endif