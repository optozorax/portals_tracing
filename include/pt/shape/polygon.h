#ifndef PT_POLYGON_H
#define PT_POLYGON_H

#include <vector>
#include <pt/object.h>

namespace pt
{

	class Polygon : public Shape
	{
	public:
		Polygon(const std::vector<vec2>& polygon, crd3 coords, Material_ptr material);

		void assign(const std::vector<vec2>& polygon, crd3 coords, Material_ptr material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		std::vector<vec2> array;
		crd3 coords;
		
		double d;
		vec3 normal;
		vec2 min, max;
	};

	inline Object_ptr makePolygon(const std::vector<vec2>& polygon, crd3 coords, Material_ptr material) { return Object_ptr(new Polygon(polygon, coords, material)); }

};

#endif