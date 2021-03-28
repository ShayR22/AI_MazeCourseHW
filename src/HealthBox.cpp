#include "HealthBox.hpp"
#include "Consumable.hpp"
#include "Drawer.hpp"


HealthBox::HealthBox(Cell* location,int healthAmount, bool isHidden): Consumable(location, ConsumableType::HEALTH, isHidden)
{
	if (healthAmount > MAX_HEALTH || healthAmount < 0)
		this->healthAmount = MAX_HEALTH;
	else
		this->healthAmount = healthAmount;
}

bool HealthBox::canConsume(Bot& p)
{
	if (p.isFullHealth())
		return false;
	else
		return true;
}

void HealthBox::consume(Bot& p)
{
	if (canConsume(p)) {
		this->setHidden(true);
		this->sethealthAmount(0);
	}

}

void HealthBox::draw(float offsetX, float offsetY)
{
	if (isHidden)
		return;

	float x = offsetX + location->getX();
	float y = offsetY + location->getY();
	Drawer::rect(x, y, 1.0f, 1.0f, DrawerColor::RED);
}