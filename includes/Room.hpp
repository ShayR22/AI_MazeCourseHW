#ifndef __ROOM__
#define __ROOM__

#include "BoardCells.hpp"
#include "Cell.hpp"
#include "Corridor.hpp"
#include "Obstacle.hpp"
#include "Consumable.hpp"
#include "vec2.h"
#include <map>
#include <vector>

class Room : public BoardCells {
private:
	std::map<Cell*, Corridor*> corridorConnections;
	std::vector<Obstacle*> obstacles;
	std::vector<Consumable*> ammoBoxes;
	std::vector<Consumable*> healthBoxes;
public:
	Room(CellMat& cells, vec2f& xyOffset);
	Room(std::map<Cell*, Corridor*> corridorConnections, CellMat& cells,
		std::vector<Obstacle*> obstacles, std::vector<Consumable*> ammoBoxes,
		std::vector<Consumable*> healthBoxes, vec2f &xyOffset);

	virtual Cell* getConnectingCell(BoardCells& board);
	virtual std::vector<BoardCells*> getConnectors();
	virtual std::vector<Consumable*> getConsumables(ConsumableType type);

	void addAmmoBox(int cellX, int cellY, int numBullets, int numGrenades, bool isHidden);
	void addHealthBox(int cellX, int cellY, int healthAmount, bool isHidden);
	void addWall(int healthPoints, int destroyFrame, std::vector<Cell*>& cover);

	void draw();
	bool isRelated2CellLocation(vec2f& cellLocation);
	float getWidth() { return static_cast<float>(cells[0].size()); }
	float getHeight() { return static_cast<float>(cells.size()); }
	void getShape(std::vector<vec2f>& points);

	inline void setCorridorConnections(std::map<Cell*, Corridor*> corridorConnections) { this->corridorConnections = corridorConnections; }
	inline std::map<Cell*, Corridor*>& getCorridorConnections() { return corridorConnections; }

	inline void setObstacles() { this->obstacles = obstacles; }
	inline std::vector<Obstacle*>& getObstacles() { return obstacles; }

	inline void setAmmoBoxes() { this->ammoBoxes = ammoBoxes; }
	inline std::vector<Consumable*>& getAmmoBoxes() { return ammoBoxes; }

	inline void setHealthBoxes() { this->healthBoxes = healthBoxes; }
	inline std::vector<Consumable*>& getHealthBoxes() { return healthBoxes; }
};

#endif