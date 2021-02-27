#ifndef __SUPPORTBOT__
#define __SUPPORTBOT__

#include "Bot.hpp"

class SupportBot : public Bot {
private:
	int ammoRefreshTimeOutMs;
	int healthRefreshTimeOutMs;
public:
	inline int getAmmoRefreshTimeOutMs() { return ammoRefreshTimeOutMs; }
	inline int getHealthRefreshTimeOutMs() { return healthRefreshTimeOutMs; }

	inline void setAmmoRefreshTimeOutMs(int ammoRefreshTimeOutMs) { this->ammoRefreshTimeOutMs = ammoRefreshTimeOutMs; }
	inline void setHealthRefreshTimeOutMs(int healthRefreshTimeOutMs) { this->healthRefreshTimeOutMs = healthRefreshTimeOutMs; }
};

#endif