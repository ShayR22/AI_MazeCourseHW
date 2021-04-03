#include "Team.hpp"
#include "SupportBot.hpp"
#include "Projectile.hpp"

Team::Team() : teamColor(DrawerColor::WHITE)
{

}

#include <iostream>
using namespace std;
static int counter = 0;

void Team::registerProjectile(Projectile& projectile)
{
	cout << __func__ << ": " << counter++ << endl;
	fflush(stdout);
	projectiles.push_back(&projectile);
}

void Team::unregisterProjectile(Projectile* projectile)
{
	cout << __func__ << ": " << counter-- << endl;
	bool found = std::find(projectiles.begin(), projectiles.end(), projectile) != projectiles.end();

	if (found) {
		projectiles.erase(remove(projectiles.begin(), projectiles.end(), projectile), projectiles.end());
		delete projectile;
	}
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

void Team::removeBot(Bot* bot)
{
	bool found = std::find(bots.begin(), bots.end(), bot) != bots.end();

	if (found) {
		bots.erase(remove(bots.begin(), bots.end(), bot), bots.end());
		delete bot;
	}

}

void Team::removeMissedProjectiles()
{
	auto it = projectiles.begin();
	while (it != projectiles.end())
	{
		/* only advance iterator if not erased */
		if ((*it)->isAtTarget()) {
			Projectile* p = (*it);
			it = projectiles.erase(it);
			delete p;
		}
		else {
			++it;
		}
	}
}

void Team::update()
{
	/* vector of projectiles that require to create other projectiles,
	 * thus, in order to register the projectiles they will create after
	 * updating the projeciles (can't add elements to vector as u iterate it)
	 */
	vector<Projectile*> requireCreation;
	for (auto& p : projectiles) {
		p->update();
		if (p->getRequireCeation())
			requireCreation.push_back(p);
	}

	for (auto& p : requireCreation) {
		p->creation();
	}

	for (auto& b : bots) {
		b->update();
	}

	removeMissedProjectiles();
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