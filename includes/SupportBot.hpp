#ifndef __SUPPORTBOT__
#define __SUPPORTBOT__

#include "Bot.hpp"

class SupportBot : public Bot {
private:
	int ammoRefreshTimeOutMs;
	int healthRefreshTimeOutMs;
public:
	SupportBot(int health, int numBullets, int numGrenades, Team& team, vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board);

	inline int getAmmoRefreshTimeOutMs() { return ammoRefreshTimeOutMs; }
	inline int getHealthRefreshTimeOutMs() { return healthRefreshTimeOutMs; }

	inline void setAmmoRefreshTimeOutMs(int ammoRefreshTimeOutMs) { this->ammoRefreshTimeOutMs = ammoRefreshTimeOutMs; }
	inline void setHealthRefreshTimeOutMs(int healthRefreshTimeOutMs) { this->healthRefreshTimeOutMs = healthRefreshTimeOutMs; }
};

#endif