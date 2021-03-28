#ifndef __CONSUMABLE__
#define __CONSUMABLE__

#include "Cell.hpp"

class Bot;

enum class ConsumableType {
	AMMO,
	HEALTH
};

class Consumable {
protected:
	Cell* location;
	ConsumableType type;
	bool isHidden;
public:
	Consumable(Cell* location, ConsumableType type, bool isHidden);

	virtual bool canConsume(Bot& p);
	virtual void consume(Bot& p);
	virtual void draw(float offsetX, float offsetY) = 0;

	inline void setlocation(Cell* location) { this->location = location; }
	inline Cell* getLocation() { return location; }

	inline void setHidden(bool isHidden) { this->isHidden = isHidden; }
	inline bool getHidden() { return isHidden; }

	inline void setConsumableType(ConsumableType type) { this->type = type; }
	inline ConsumableType getConsumableType() { return type; }
};

#endif
