#include "CollisionLogic.hpp"

double CollisionLogic::getCollisionDistance(vec2f& p, vec2f& dir, std::vector<vec2f>& points) {
	Line2D line;
	double dist = DBL_MAX;
	double curDist;

	size_t s = points.size();
	for (size_t i = 0; i < s; ++i) {
		line.set(points[i % s], points[(i + 1) % s]);
		curDist = getDistTilCollidLine(p, dir, line);
		dist = fmin(curDist, dist);
	}

	return dist;
}

double CollisionLogic::getDistTilCollidLine(vec2f& point, vec2f& dir, Line2D& line) {
	vec2f targetPoint = point + dir;
	Line2D vecLine(point, targetPoint);

	if (vecLine.isParallelTo(line))
		return DBL_MAX;

	vec2f intersectPoint;
	Line2D::getIntersectPoint(line, vecLine, intersectPoint);

	vec2f dirToLine = intersectPoint - point;
	if (!dir.hasSameDirection(dirToLine))
		return DBL_MAX;

	if (!line.isInLineLimits(intersectPoint))
		return DBL_MAX;

	return point.dist(intersectPoint);
}