#ifndef __SUPPORTBOT__
#define __SUPPORTBOT__

#include "Bot.hpp"

#define MAX_DIST_FOR_SUPPORTING 2.0f

class SupportBot : public Bot {
private:
	int ammoRefreshTimeOutMs;
	int healthRefreshTimeOutMs;
	bool canGiveAmmo;
	bool canGiveHealth;
	std::chrono::high_resolution_clock::time_point lastAmmoGiven;
	std::chrono::high_resolution_clock::time_point lastHealthGiven;


	void updateLastConsumabeGiven(std::chrono::time_point<std::chrono::steady_clock>& now,
							std::chrono::high_resolution_clock::time_point& lastConsumeGiven,
							bool& canGiveConsumable, int timeoutMS);

	void giveHealth(Bot& bot);
	void giveAmmo(Bot& bot);
	void supportOtherBots();
public:
	SupportBot(int health, int numBullets, int numGrenades, Team& team, vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingDiameter, BoardCells& board);

	virtual void update();
	virtual void draw();


	inline int getAmmoRefreshTimeOutMs() { return ammoRefreshTimeOutMs; }
	inline int getHealthRefreshTimeOutMs() { return healthRefreshTimeOutMs; }

	inline void setAmmoRefreshTimeOutMs(int ammoRefreshTimeOutMs) { this->ammoRefreshTimeOutMs = ammoRefreshTimeOutMs; }
	inline void setHealthRefreshTimeOutMs(int healthRefreshTimeOutMs) { this->healthRefreshTimeOutMs = healthRefreshTimeOutMs; }

	inline bool getCanGiveAmmo() { return canGiveAmmo; }
	inline bool setCanGiveAmmo(bool canGiveAmmo) { this->canGiveAmmo = canGiveAmmo; }

	inline bool getCanGiveHealth() { return canGiveHealth; }
	inline bool setCanGiveHealth(bool canGiveHealth) { this->canGiveHealth = canGiveHealth; }
};

#endif