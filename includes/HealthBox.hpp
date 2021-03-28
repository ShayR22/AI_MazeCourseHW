#ifndef __HEALTHBOX__
#define __HEALTHBOX__

#include "Consumable.hpp"
#include "Bot.hpp"

class HealthBox : public Consumable {
private:
	int healthAmount;
public:
	HealthBox(Cell* loctaion,int healthAmount, bool isHidden);

	virtual bool canConsume(Bot& p);
	virtual void consume(Bot& p);
	virtual void draw(float offsetX, float offsetY);

	inline void sethealthAmount(int healthAmount) { this->healthAmount = healthAmount;}
	inline int gethealthAmount() { return healthAmount;}
};

#endif