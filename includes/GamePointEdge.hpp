#ifndef __GAME_POINT_EDGE__
#define __GAME_POINT_EDGE__

#include <cmath>
#include "GamePoint.hpp"
#include "GamePointNode.hpp"
#include "heruistics.h"

class GamePointEdge {
public:
	GamePointNode* from;
	GamePointNode* to;
	double weight;
	GamePointEdge(GamePointNode* from, GamePointNode* to) : from(from), to(to), weight(manhattan_distance(from, to)) {}
	GamePointEdge(const GamePointEdge& other) : from(other.from), to(other.to), weight(other.weight) {}

	inline GamePointNode* getFrom() { return from; }
	inline void setFrom(GamePointNode* from) { this->from = from; }
	inline GamePointNode* getTo() { return to; }
	inline void setTo(GamePointNode* to) { this->to = to; }
};

#endif

