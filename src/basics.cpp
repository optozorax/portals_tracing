#include <random>
#include <math.h>
#include <pt/basics.h>

namespace pt
{

std::mt19937 generator;
std::uniform_real_distribution<double> distribution(0, 1);

//-----------------------------------------------------------------------------
double random(void) {
	return distribution(generator);
}

//-----------------------------------------------------------------------------
Color Color::operator/=(double k) {
	r /= k;
	g /= k;
	b /= k;
	a /= k;
	return *this;
}

//-----------------------------------------------------------------------------
Color overlay(const Color& upper, const Color& lower) {
	Color out;
	out.a = upper.a + lower.a * (1 - upper.a);
	out.r = (upper.r * upper.a + lower.r * lower.a * (1 - upper.a))/out.a;
	out.g = (upper.g * upper.a + lower.g * lower.a * (1 - upper.a))/out.a;
	out.b = (upper.b * upper.a + lower.b * lower.a * (1 - upper.a))/out.a;
	return out;
}

//-----------------------------------------------------------------------------
Color Color::operator*(const Color& a) {
	Color clr = *this;
	clr.a = clr.a + a.a*(1 - clr.a);
	clr.r *= a.r;
	clr.g *= a.g;
	clr.b *= a.b;
	return clr;
}

//-----------------------------------------------------------------------------
Color Color::operator+=(const Color& clr) {
	a += clr.a;
	r += clr.r;
	g += clr.g;
	b += clr.b;
	return *this;
}

//-----------------------------------------------------------------------------
Color Color::operator*(double a) {
	Color clr = *this;
	clr.r *= a;
	clr.g *= a;
	clr.b *= a;
	return clr;
}

//-----------------------------------------------------------------------------
Color Color::sqrt(void) {
	r = ::sqrt(r);
	g = ::sqrt(g);
	b = ::sqrt(b);
	return *this;
}

//-----------------------------------------------------------------------------
Vector toCoordSystem(const CoordSystem& newsys, Vector p) {
	Vector pnew;
	p -= newsys.pos;
	pnew.x = dot(p, newsys.i);
	pnew.y = dot(p, newsys.j);
	pnew.z = dot(p, newsys.k);
	return pnew;
}

//-----------------------------------------------------------------------------
Vector fromCoordSystem(const CoordSystem& current, const Vector& p) {
	return current.pos + current.i * p.x + current.j * p.y + current.k * p.z;
}

//-----------------------------------------------------------------------------
Vector teleportVector(const CoordSystem& first, const CoordSystem& second, const Vector& absolutePos) {
	Vector inFirst = toCoordSystem(first, absolutePos);
	return fromCoordSystem(second, inFirst);
}

//-----------------------------------------------------------------------------
Vector teleportDirection(const CoordSystem& first,
						 const CoordSystem& second,
						 const Vector& direction) {
	return teleportVector(first, second, first.pos + direction) - second.pos;
}

//-----------------------------------------------------------------------------
CoordSystem teleportCoordSystem(const CoordSystem& first, const CoordSystem& second, const CoordSystem& p) {
	CoordSystem newsys;
	newsys.pos = teleportVector(first, second, p.pos);
	newsys.i = teleportDirection(first, second, p.i);
	newsys.j = teleportDirection(first, second, p.j);
	newsys.k = teleportDirection(first, second, p.k);
	return newsys;
}

//-----------------------------------------------------------------------------
CoordSystem calculateCoordSystem(const Vector& a, const Vector& b, const Vector& c) {
	CoordSystem newsys;
	newsys.pos = a;
	newsys.i = (b - a).normalize();
	newsys.j = c - a;
	newsys.j = (newsys.j - newsys.i*dot(newsys.j, newsys.i)).normalize();
	newsys.k = cross(newsys.j, newsys.i).normalize();
	return newsys;
}

//-----------------------------------------------------------------------------
bool isRightCoordSystem(const CoordSystem& coords) {
	return dot(cross(coords.i, coords.j), coords.k) > 0;
}

//-----------------------------------------------------------------------------
void rotate2(double& x, double &y, const double& angle) {
	double x1 = cos(angle)*x - sin(angle)*y;
	double y1 = sin(angle)*x + cos(angle)*y;
	x = x1;
	y = y1;
}

//-----------------------------------------------------------------------------
Vector rotate(Vector a, const Vector& angles) {
	rotate2(a.x, a.y, angles.x);
	rotate2(a.x, a.z, angles.z);
	rotate2(a.y, a.z, -angles.y);
	return a;
}
//-----------------------------------------------------------------------------
CoordSystem rotate(const CoordSystem& coords, Vector angles) {
	CoordSystem newsys;
	newsys.pos = coords.pos;
	newsys.i = rotate(coords.i, angles);
	newsys.j = rotate(coords.j, angles);
	newsys.k = rotate(coords.k, angles);
	return newsys;
}

//-----------------------------------------------------------------------------
/** Big thanks to M. Galetzka and P. Glauner. This code was copied from https://github.com/pglauner/point_in_polygon . */

//-----------------------------------------------------------------------------
struct Line {
	Vector2 p1;
	Vector2 p2;
};

//-----------------------------------------------------------------------------
static int ccw(Vector2 p0, Vector2 p1, Vector2 p2) {
	double dx1 = p1.x - p0.x;
	double dy1 = p1.y - p0.y;
	double dx2 = p2.x - p0.x;
	double dy2 = p2.y - p0.y;

	// second slope is greater than the first one --> counter-clockwise
	if (dx1 * dy2 > dx2 * dy1) {
		return 1;
	}
	// first slope is greater than the second one --> clockwise
	else if (dx1 * dy2 < dx2 * dy1) {
		return -1;
	}
	// both slopes are equal --> collinear line segments
	else {
		// p0 is between p1 and p2
		if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
			return -1;
		}
		// p2 is between p0 and p1, as the length is compared
		// square roots are avoided to increase performance
		else if (dx1 * dx1 + dy1 * dy1 >= dx2 * dx2 + dy2 * dy2) {
			return 0;
		}
		// p1 is between p0 and p2
		else {
			return 1;
		}
	}
}

//-----------------------------------------------------------------------------
static int intersect(Line line1, Line line2) {
	// ccw returns 0 if two points are identical, except from the situation
	// when p0 and p1 are identical and different from p2
	double ccw11 = ccw(line1.p1, line1.p2, line2.p1);
	double ccw12 = ccw(line1.p1, line1.p2, line2.p2);
	double ccw21 = ccw(line2.p1, line2.p2, line1.p1);
	double ccw22 = ccw(line2.p1, line2.p2, line1.p2);

	return (((ccw11 * ccw12 < 0) && (ccw21 * ccw22 < 0))
			// one ccw value is zero to detect an intersection
			|| (ccw11 * ccw12 * ccw21 * ccw22 == 0)) ? 1 : 0;
}

//-----------------------------------------------------------------------------
static int getNextIndex(int n, int current) {
	return current == n - 1 ? 0 : current + 1;
}

//-----------------------------------------------------------------------------
bool pointInPolygon(const std::vector<Vector2>& polygon1, Vector2 testPoint) {
	auto polygon = polygon1;
	int n = polygon.size();
	Line xAxis;
	Line xAxisPositive;

	Vector2 startPoint;
	Vector2 endPoint;
	Line edge;
	Line testPointLine;

	int i;
	double startNodePosition;
	int count;
	int seenPoints;

	// Initial start point
	startPoint.x = 0;
	startPoint.y = 0;

	// Create axes
	xAxis.p1.x = 0;
	xAxis.p1.y = 0;
	xAxis.p2.x = 0;
	xAxis.p2.y = 0;
	xAxisPositive.p1.x = 0;
	xAxisPositive.p1.y = 0;
	xAxisPositive.p2.x = 0;
	xAxisPositive.p2.y = 0;

	startNodePosition = -1;

	// Is testPoint on a node?
	// Move polygon to 0|0
	// Enlarge axes
	for (i = 0; i < n; i++) {
		if (testPoint.x == polygon[i].x && testPoint.y == polygon[i].y) {
			return 1;
		}

		// Move polygon to 0|0
		polygon[i].x -= testPoint.x;
		polygon[i].y -= testPoint.y;

		// Find start point which is not on the x axis
		if (polygon[i].y != 0) {
			startPoint.x = polygon[i].x;
			startPoint.y = polygon[i].y;
			startNodePosition = i;
		}

		// Enlarge axes
		if (polygon[i].x > xAxis.p2.x) {
			xAxis.p2.x = polygon[i].x;
			xAxisPositive.p2.x = polygon[i].x;
		}
		if (polygon[i].x < xAxis.p1.x) {
			xAxis.p1.x = polygon[i].x;
		}
	}

	// Move testPoint to 0|0
	testPoint.x = 0;
	testPoint.y = 0;
	testPointLine.p1 = testPoint;
	testPointLine.p2 = testPoint;

	// Is testPoint on an edge?
	for (i = 0; i < polygon.size(); i++) {
		edge.p1 = polygon[i];
		// Get correct index of successor edge
		edge.p2 = polygon[getNextIndex(polygon.size(), i)];
		if (intersect(testPointLine, edge) == 1) {
			return 1;
		}
	}

	// No start point found and point is not on an edge or node
	// --> point is outside
	if (startNodePosition == -1) {
		return 0;
	}

	count = 0;
	seenPoints = 0;
	i = startNodePosition;

	// Consider all edges
	while (seenPoints < n) {

		double savedX = polygon[getNextIndex(n, i)].x;
		int savedIndex = getNextIndex(n, i);

		// Move to next point which is not on the x-axis
		do {
			i = getNextIndex(n, i);
			seenPoints++;
		} while (polygon[i].y == 0);
		// Found end point
		endPoint.x = polygon[i].x;
		endPoint.y = polygon[i].y;

		// Only intersect lines that cross the x-axis
		if (startPoint.y * endPoint.y < 0) {
			edge.p1 = startPoint;
			edge.p2 = endPoint;

			// No nodes have been skipped and the successor node
			// has been chosen as the end point
			if (savedIndex == i) {
				count += intersect(edge, xAxisPositive);
			}
			// If at least one node on the right side has been skipped,
			// the original edge would have been intersected
			// --> intersect with full x-axis
			else if (savedX > 0) {
				count += intersect(edge, xAxis);
			}
		}
		// End point is the next start point
		startPoint = endPoint;
	}

	// Odd count --> in the polygon (1)
	// Even count --> outside (0)
	return count % 2;
}

};