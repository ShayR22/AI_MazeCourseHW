#include "Line.h"
#include "vec2.h"
#include <limits>

Line2D::Line2D() : p1(FLT_MAX, FLT_MAX), p2(FLT_MAX, FLT_MAX)
{
}

Line2D::Line2D(vec2f& p1, vec2f& p2) : p1(p1), p2(p2) 
{
}

void Line2D::getFormalEquasion(float& a, float& b, float& c) const {
	// a, b, c are the coefficients of y, x, C in the formal line - equasion(see Wikipedia)
	float x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
	a = x1 - x2;
	b = y2 - y1;
	c = (x2 - x1) * y1 + (y1 - y2) * x1;
}

void Line2D::set(vec2f& p1, vec2f& p2)
{
	this->p1 = p1;
	this->p2 = p2;
}

bool Line2D::isParallelTo(Line2D& other) {
	float a0, b0, c0, a1, b1, c1;
	getFormalEquasion(a0, b0, c0);
	other.getFormalEquasion(a1, b1, c1);

	if (a0 != 0 && a1 != 0) {
		return fabs((b1 / a1) - (b0 / a0)) < EPSILON;
	}
	else if (a0 == 0 && a1 == 0) {
		return true;
	}
	else {
		return false;
	}
}

void Line2D::getIntersectPoint(Line2D& line1, Line2D& line2, vec2f& intersectPoint) {
	// for intersection-point calc, we use determinations. From:
	// https://stackoverflow.com/questions/31756413/solving-a-simple-matrix-in-row-reduced-form-in-c
	float matrix[2][3];
	float a1, b1, c1;
	float a2, b2, c2;

	line1.getFormalEquasion(a1, b1, c1);
	line2.getFormalEquasion(a2, b2, c2);

	if (a1 != 0) {
		matrix[0][0] = a1; matrix[0][1] = b1; matrix[0][2] = -c1;
		matrix[1][0] = a2; matrix[1][1] = b2; matrix[1][2] = -c2;
	}
	else {
		matrix[0][0] = a2; matrix[0][1] = b2; matrix[0][2] = -c2;
		matrix[1][0] = a1; matrix[1][1] = b1; matrix[1][2] = -c1;
	}

	rowReduce(matrix);

	intersectPoint.set(matrix[1][2], matrix[0][2]);
}

void Line2D::rowReduce(float A[][3])
{
	const int numRows = 2;
	const int numCols = 3;

	int lead = 0;

	while (lead < numRows) {
		float d, m;

		for (int r = 0; r < numRows; r++) { // for each row ...
			/* calculate divisor and multiplier */
			d = A[lead][lead];
			m = A[r][lead] / A[lead][lead];

			for (int c = 0; c < numCols; c++) { // for each column ...
				if (r == lead)
					A[r][c] /= d;               // make pivot = 1
				else
					A[r][c] -= A[lead][c] * m;  // make other = 0
			}
		}

		lead++;
	}
}

bool Line2D::isInLineLimits(vec2f& point) {
	float lineLength = p1.dist(p2);
	float pointDistsSum = p1.dist(point) + point.dist(p2);
	return  fabs(lineLength - pointDistsSum) < EPSILON;
}
