#include "AmmoBox.hpp"
#include "Consumable.hpp"
#include "Drawer.hpp"


AmmoBox::AmmoBox(Cell* loctaion, int ammoAmount, bool isHidden): Consumable(loctaion, ConsumableType::AMMO, isHidden)
{
	if (ammoAmount > MAX_AMMO || ammoAmount < 0)
		this->ammoAmount = MAX_AMMO;
	else
		this->ammoAmount = ammoAmount;
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
		this->setHidden(true);
		this->setammoAmount(0);
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


