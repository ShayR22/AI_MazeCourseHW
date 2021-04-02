#include "Team.hpp"
#include "SupportBot.hpp"

Team::Team() : teamColor(DrawerColor::WHITE)
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

void Team::setTeamColor(DrawerColor teamColor)
{
	this->teamColor = teamColor;
	for (auto& bot : bots) {
		bot->setTeamColor(teamColor);
	}
}

void Team::draw()
{
	for (auto& bot : bots) {
		bot->draw();
	}

	for (auto& p : projectiles) {
		p->draw();
	}
}