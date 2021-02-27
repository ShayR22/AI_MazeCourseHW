#ifndef __BOARDCELLS__
#define __BOARDCELLS__

#include "Cell.hpp"
#include "Consumable.hpp"
#include <vector>

class BoardCells {
protected:
	bool shootable;
	std::vector<float> xyOffset; // where corridor is in reference in the game, for drawing
public:
	BoardCells(bool shootable, std::vector<float> xyOffset);

	virtual std::vector<std::vector<Cell>>& getCells() = 0;
	virtual Cell* getConnectingCell(BoardCells& board) = 0;
	virtual std::vector<BoardCells*> getConnectors() = 0;
	virtual std::vector<Consumable>& getConsumables(ConsumableType type) = 0;

	inline void setShootable() { this->shootable = shootable; }
	inline bool getShootable() { return shootable; }

	inline void setXYOffset() { this->xyOffset = xyOffset; }
	inline std::vector<float>& getXYOffset() { return xyOffset; }
};

#endif