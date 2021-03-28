#include "Cell.hpp"
#include "Bot.hpp"

Consumable::Consumable(Cell* location, ConsumableType type, bool isHidden) : location(location) , type(type), isHidden(false)
{

}

bool Consumable::canConsume(Bot& p)
{
	if (type == ConsumableType::AMMO)
	{
		if (p.isAmmoFull())
			return false;
		else
			return true;
	}
	else if (type == ConsumableType::HEALTH)
	{
		if (p.isFullHealth())
			return false;
		else
			return true;
	}
}

void Consumable::consume(Bot& p)
{
	if (canConsume(p))
		this->setHidden(true);
}

