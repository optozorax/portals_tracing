#include <pt/shape/polygon.h>

namespace pt
{

//-----------------------------------------------------------------------------
Polygon::Polygon(std::vector<Point> polygon, Coords coords, Material* material) : array(polygon), coords(coords), Shape(material) {
	d = -dot(coords.k, coords.pos);
	normal = coords.k;

	min.x = +std::numeric_limits<double>::infinity();
	min.y = +std::numeric_limits<double>::infinity();
	max.x = -std::numeric_limits<double>::infinity();
	max.y = -std::numeric_limits<double>::infinity();

	for (size_t i = 0; i < polygon.size(); i++) {
		if (polygon[i].x < min.x) min.x = polygon[i].x;
		if (polygon[i].y < min.y) min.y = polygon[i].y;
		if (polygon[i].x > max.x) max.x = polygon[i].x;
		if (polygon[i].y > max.y) max.y = polygon[i].y;
	}
}

//-----------------------------------------------------------------------------
struct Line {
	Point p1;
	Point p2;
};

//-----------------------------------------------------------------------------
static int ccw(Point p0, Point p1, Point p2) {
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
bool inPolygon(const std::vector<Point>& polygon1, Point testPoint) {
	auto polygon = polygon1;
	int n = polygon.size();
	Line xAxis;
	Line xAxisPositive;

	Point startPoint;
	Point endPoint;
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

//-----------------------------------------------------------------------------
bool Polygon::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	if (dot(ray.dir, normal)) {
		double t = (-d - dot(ray.pos, normal))/dot(ray.dir, normal);

		if (t > tMin && t < tMax) {
			// Position when ray intersect plane
			Vector x = ray.pos + ray.dir * t;

			Point r;
			r.x = (dot(x, coords.i) - dot(coords.pos, coords.i))/dot(coords.i, coords.i);
			r.y = (dot(x, coords.j) - dot(coords.pos, coords.j))/dot(coords.j, coords.j);

			bool inRect = r.x >= min.x && r.y >= min.y && r.x <= max.x && r.y <= max.y;
			//bool inRect = true;

			if (inRect && inPolygon(array, r)) {
				inter.t = t;
				if (dot(ray.dir, normal) < 0)
					inter.normal = normal;
				else
					inter.normal = -normal;
				inter.pos = x;
				return true;
			} else
				return false;
		} else
			// Ray is not in ranges
			return false;
	} else {
		// Ray is parallel plane
		return false;
	}
}

};