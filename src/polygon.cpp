#include <limits>
#include <pt/shape/polygon.h>

namespace pt
{

//-----------------------------------------------------------------------------
Polygon::Polygon(const std::vector<vec2>& polygon, crd3 coords1, Material_ptr material) : Shape(material) {
	assign(polygon, coords1, material);
}

//-----------------------------------------------------------------------------
void Polygon::assign(const std::vector<vec2>& polygon, crd3 coords1, Material_ptr material1) {
	array = polygon;
	coords = coords1;
	material = material1;

	d = -dot(coords.k, coords.pos);
	normal = coords.k;
	normal = normal.normalize();

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
bool Polygon::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	if (dot(ray.dir, normal) != 0) {
		double t = (-d - dot(ray.pos, normal))/dot(ray.dir, normal);

		if (t > tMin && t < tMax) {
			// Position when ray intersect plane
			vec3 x = ray.pos + ray.dir * t;

			vec2 r;
			r.x = (dot(x, coords.i) - dot(coords.pos, coords.i))/dot(coords.i, coords.i);
			r.y = (dot(x, coords.j) - dot(coords.pos, coords.j))/dot(coords.j, coords.j);

			bool inRect = r.x >= min.x && r.y >= min.y && r.x <= max.x && r.y <= max.y;
			if (!inRect)
				return false;

			if (pointInPolygon(array, r)) {
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