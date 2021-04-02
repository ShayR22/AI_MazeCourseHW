#ifndef __AMMOBOX__
#define __AMMOBOX__

#include "Consumable.hpp"
#include "Bot.hpp"

class AmmoBox : public Consumable {
private:
	int numBullets;
	int numGrenades;
public:
	AmmoBox(Cell* loctaion,int numBullets, int numGrenades, bool isHidden);

	virtual bool canConsume(Bot& p);
	virtual void consume(Bot& p);
	virtual void draw(float offsetX, float offsetY);

	inline void setNumBullets(int numBullets) { this->numBullets = numBullets;}
	inline int getNumBullets() { return numBullets;}

	inline void setNumGrenades(int numGrenades) { this->numGrenades = numGrenades; }
	inline int getNumGrenades() { return numGrenades; }
};

#endif