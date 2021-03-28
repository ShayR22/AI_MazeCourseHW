#ifndef __AMMOBOX__
#define __AMMOBOX__

#include "Consumable.hpp"
#include "Bot.hpp"

class AmmoBox : public Consumable {
private:
	int ammoAmount;
public:
	AmmoBox(Cell* loctaion,int ammoAmount, bool isHidden);

	virtual bool canConsume(Bot& p);
	virtual void consume(Bot& p);
	virtual void draw(float offsetX, float offsetY);

	inline void setammoAmount(int ammoAmount) { this->ammoAmount = ammoAmount;}
	inline int getammoAmount() { return ammoAmount;}
};

#endif