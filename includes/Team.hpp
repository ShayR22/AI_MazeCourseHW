#ifndef __TEAM__
#define __TEAM__

#include <vector>
#include "Projectile.hpp"
#include "Drawer.hpp"

class Bot;
class SupportBot;

class Team {
private:
	std::vector<Projectile*> projectiles;
	std::vector<Bot*> bots;
	DrawerColor teamColor;

	void removeMissedProjectiles();
public:
	Team();
	void registerProjectile(Projectile& projectile);
	void unregisterProjectile(Projectile* projectile);
	SupportBot* isSupportBotAlive(Bot& callingBot);

	void update();

	inline std::vector<Projectile*> getProjectiles() { return projectiles; }
	inline std::vector<Bot*> getBots() { return bots; }

	inline void setProjectiles(std::vector<Projectile*> projectiles) { this->projectiles = projectiles; }
	inline void setBots(std::vector<Bot*> bots) { this->bots = bots; }

	void setTeamColor(DrawerColor teamColor);
	inline DrawerColor getTeamColor() { return teamColor; }

	void removeBot(Bot* bot);

	void draw();
};

#endif