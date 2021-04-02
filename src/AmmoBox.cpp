#include "AmmoBox.hpp"
#include "Consumable.hpp"
#include "Drawer.hpp"


AmmoBox::AmmoBox(Cell* loctaion, int numBullets, int numGrenades, bool isHidden) : numBullets(numBullets), numGrenades(numGrenades),
Consumable(loctaion, ConsumableType::AMMO, isHidden)
{
	if (numBullets > MAX_BULLETS || numBullets < 0)
		this->numBullets = MAX_BULLETS;
	else
		this->numBullets = numBullets;

	if (numGrenades > MAX_GRENADES || numGrenades < 0)
		this->numGrenades = MAX_GRENADES;
	else
		this->numGrenades = numGrenades;
}

bool AmmoBox::canConsume(Bot& p)
{	
	if (p.isAmmoFull())
		return false;
	else
		return true;
}

void AmmoBox::consume(Bot& p)
{
	if (canConsume(p)) {
		p.setNumBullets(MAX_BULLETS);
		p.setNumGrenades(MAX_GRENADES);
		this->setHidden(true);
		this->setNumBullets(0);
		this->setNumGrenades(0);
	}
}

void AmmoBox::draw(float offsetX, float offsetY)
{
	if (isHidden)
		return;

	float x = offsetX + location->getX();
	float y = offsetY + location->getY();
	Drawer::rect(x, y, 1.0f, 1.0f, DrawerColor::BLUE);
}


