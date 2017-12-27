#ifndef PT_POLYGON_H
#define PT_POLYGON_H

#include <vector>
#include <pt/object.h>

namespace pt
{

	class Polygon : public Shape
	{
	public:
		Polygon(const Poly2& polygon, CoordSystem coords, Material* material);

		void assign(const Poly2& polygon, CoordSystem coords, Material* material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		std::vector<Vector2> array;
		CoordSystem coords;
		
		double d;
		Vector normal;
		Vector2 min, max;
	};

};

#endif