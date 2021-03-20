#ifndef __LINE
#define __LINE

#include "vec2.h"

class Line2D 
{
public:
	Line2D();
	Line2D(vec2f& p1, vec2f& p2);
	void getFormalEquasion(float& a, float& b, float& c) const;
	void set(vec2f& p1, vec2f& p2);
	bool isParallelTo(Line2D& other);
	bool isInLineLimits(vec2f& point);
	static void getIntersectPoint(Line2D& line1, Line2D& line2, vec2f& intersectPoint);

private:
	static void rowReduce(float matrix[][3]);

private:
	vec2f p1, p2;
};

#endif // __LINE