#ifndef PT_PORTALS_H
#define PT_PORTALS_H

#include <pt/vector.h>
#include <pt/object.h>
#include <pt/shape/polygon.h>

namespace pt
{

	//-------------------------------------------------------------------------
	class Portals : public Object
	{
	public:
		Portals(CoordSystem c1, CoordSystem c2, Poly2 poly, Material* first, Material* second);
		~Portals();

		void assign(CoordSystem c1, CoordSystem c2, Poly2 poly, Material* first, Material* second);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;
		Polygon pg1;
		Polygon pg2;
		CoordSystem p1;
		CoordSystem p2;
		Poly2 poly;
		Material* first;
		Material* second;
		mutable int currentTriangle;
	};

};

#endif