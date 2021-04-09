#include "Cell.hpp"
#include "Bot.hpp"

Consumable::Consumable(Cell* location, ConsumableType type, bool isHidden) : location(location) , type(type), isHidden(false)
{

}