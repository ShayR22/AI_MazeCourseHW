#include <chrono>
#include "SupportBot.hpp"
#include "heruistics.h"

constexpr auto AMMO_REFRESH_TIMEOUT_MS = 20 * 1000;
constexpr auto HEALTH_REFRESH_TIMEOUT_MS = 5 * 1000;

using namespace std;
using namespace chrono;

SupportBot::SupportBot(int health, int numBullets, int numGrenades, Team& team, vec2f& location,
	vec2f& maxSpeed, vec2f& target, float boundingDiameter, BoardCells& board) :
	Bot(health, numBullets, numGrenades, team, location, maxSpeed, target, boundingDiameter, board),
	ammoRefreshTimeOutMs(AMMO_REFRESH_TIMEOUT_MS), healthRefreshTimeOutMs(HEALTH_REFRESH_TIMEOUT_MS),
	canGiveAmmo(true), canGiveHealth(true), lastAmmoGiven(high_resolution_clock::now()),
	lastHealthGiven(high_resolution_clock::now())
{
	/* DO NOTHING */
}

void SupportBot::giveHealth(Bot& bot)
{
	if (bot.getHealth() > HEALTH_THRESHOLD) {
		return;
	}

	bot.setHealth(MAX_HEALTH);
	canGiveHealth = false;
	lastHealthGiven = high_resolution_clock::now();
}

void SupportBot::giveAmmo(Bot& bot)
{
	if (bot.getNumBullets() > AMMO_THRESHOLD_BULLET) {
		return;
	}

	bot.setNumBullets(MAX_BULLETS);
	bot.setNumGrenades(MAX_GRENADES);
	canGiveAmmo = false;
	lastAmmoGiven = high_resolution_clock::now();
}

void SupportBot::updateLastConsumabeGiven(time_point<steady_clock>& now,
	high_resolution_clock::time_point& lastConsumeGiven, bool& canGiveConsumable,
	int timeoutMS)
{
	if (canGiveConsumable) {
		return;
	}

	duration<double, milli> timePassedMS = now - lastConsumeGiven;
	if (timePassedMS.count() > timeoutMS) {
		canGiveConsumable = true;
	}
}

void SupportBot::supportOtherBots()
{
	auto now = high_resolution_clock::now();
	updateLastConsumabeGiven(now, lastAmmoGiven, canGiveAmmo, AMMO_REFRESH_TIMEOUT_MS);
	updateLastConsumabeGiven(now, lastHealthGiven, canGiveHealth, HEALTH_REFRESH_TIMEOUT_MS);

	for (auto& bot : team.getBots()) {
	/*	if (this == bot) {
			continue;
		}*/

		float distance = manhattan_distance(this, bot);
		if (distance > MAX_DIST_FOR_SUPPORTING) {
			continue;
		}

		if (canGiveHealth) {
			giveHealth(*bot);
		}

		if (canGiveAmmo) {
			giveAmmo(*bot);
		}
	}
}

void SupportBot::update()
{
	Bot::update();
	supportOtherBots();
}

void SupportBot::draw()
{
	float x = location.x;
	float y = location.y;

	Bot::draw();
	Drawer::filledCircle(x + lookingAt.x, y + lookingAt.y, boundingDiameter / 2.0f, DrawerColor::PURPULE);
	Drawer::circle(x + lookingAt.x, y + lookingAt.y, boundingDiameter / 2.0f, DrawerColor::BLACK);

}


