#include "SupportBot.hpp"

constexpr auto AMMO_REFRESH_TIMEOUT_SEC = 10;
constexpr auto HEALTH_REFRESH_TIMEOUT_SEC = 20;

SupportBot::SupportBot(int health, int numBullets, int numGrenades, Team& team, vec2f& location,
	vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board) :
	Bot(health, numBullets, numGrenades, team, location, maxSpeed, target, boundingRadius, board),
	ammoRefreshTimeOutMs(AMMO_REFRESH_TIMEOUT_SEC), healthRefreshTimeOutMs(HEALTH_REFRESH_TIMEOUT_SEC)
{
	/* DO NOTHING */
}

