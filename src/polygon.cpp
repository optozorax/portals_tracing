#include <pt/shape/polygon.h>

namespace pt
{

//-----------------------------------------------------------------------------
Polygon::Polygon(std::vector<Point> polygon, Material* material) : array(polygon), Shape(material) {
}

//-----------------------------------------------------------------------------
bool inPolygon(std::vector<Point> polygon, Point x) {
	// @TODO make it
}

//-----------------------------------------------------------------------------
bool Polygon::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	double t = -ray.pos.z / ray.dir.z;
	if (t >= tMin && t <= tMax) {
		inter.t = t;
		inter.pos = Vector(ray.pos.x + ray.dir.x * t, ray.pos.x + ray.dir.x * t, 0);
		inter.normal = Vector(0, 0, 1);
		Point x;
		x.x = inter.pos.x;
		x.y = inter.pos.y;
		return inPolygon(array, x);
	} else
		return false
}

};