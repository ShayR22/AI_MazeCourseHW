#ifndef __TEAM__
#define __TEAM__

#include <vector>
#include "Projectile.hpp"

class Bot;

class Team {
private:
	std::vector<Projectile*> projectiles;
	std::vector<Bot*> bots;
public:
	void registerProjectile(Projectile& projectile);
	Bot* isSupportBotAlive() { return nullptr; }
	void update();

	inline std::vector<Projectile*> getProjectiles() { return projectiles; }
	inline std::vector<Bot*> getBots() { return bots; }

	inline void setProjectiles(std::vector<Projectile*> projectiles) { this->projectiles = projectiles; }
	inline void setBots(std::vector<Bot*> bots) { this->bots = bots; }
};

#endif