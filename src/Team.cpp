#include "Team.hpp"
#include "SupportBot.hpp"

Team::Team()
{

}

void Team::registerProjectile(Projectile& projectile)
{
	projectiles.push_back(&projectile);
}

Bot* Team::isSupportBotAlive()
{
	for (auto& bot : bots) {
		SupportBot* supportBot = dynamic_cast<SupportBot*>(bot);
		if (supportBot && supportBot->getHealth() > 0) {
			return bot;
		}
	}

	return nullptr;
}

void Team::update()
{
	for (auto p : projectiles) {
		p->update();
	}
}

void Team::draw()
{
	for (auto p : projectiles) {
		p->draw();
	}
}