#ifndef __BOARDCELLS__
#define __BOARDCELLS__

#include <vector>
#include "Cell.hpp"
#include "Consumable.hpp"
#include "vec2.h"

class BoardCells {
protected:
	CellMat cells;
	bool shootable;
	vec2f xyOffset; // where BoardCells is in reference in the game, for drawing
public:
	BoardCells(CellMat &cells, bool shootable, vec2f& xyOffset);

	virtual Cell* getConnectingCell(BoardCells& board) = 0;
	virtual std::vector<BoardCells*> getConnectors() = 0;
	virtual std::vector<Consumable*> getConsumables(ConsumableType type) = 0;

	inline void setShootable() { this->shootable = shootable; }
	inline bool getShootable() { return shootable; }

	inline void setCells(CellMat& cells) { this->cells = cells; }
	inline CellMat& getCells() { return cells; }

	inline void setXYOffset(vec2f& xyOffset) { this->xyOffset = xyOffset; }
	inline vec2f& getXYOffset() { return xyOffset; }
};

#endif