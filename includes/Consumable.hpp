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
	Cell* loctaion;
	ConsumableType type;
public:
	Consumable(Cell* loctaion, ConsumableType type);

	virtual bool canConsume(Bot& p);
	virtual void consume(Bot& p);
	virtual void draw(float offsetX, float offsetY) = 0;

	inline void setLoctaion() { this->loctaion = loctaion; }
	inline Cell* getLocation() { return loctaion; }

	inline void setConsumableType(ConsumableType type) { this->type = type; }
	inline ConsumableType getConsumableType() { return type; }
};

#endif
