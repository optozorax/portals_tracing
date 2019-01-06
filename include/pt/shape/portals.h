#ifndef PT_PORTALS_H
#define PT_PORTALS_H

#include <pt/object.h>
#include <pt/shape/polygon.h>

namespace pt
{

	//-------------------------------------------------------------------------
	class Portals : public Object
	{
	public:
		Portals(crd3 c1, crd3 c2, std::vector<vec2> poly, Material* first, Material* second);
		~Portals();

		void assign(crd3 c1, crd3 c2, std::vector<vec2> poly, Material* first, Material* second);

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
		crd3 p1;
		crd3 p2;
		std::vector<vec2> poly;
		Material* first;
		Material* second;
		mutable int currentTriangle;
	};

	//-------------------------------------------------------------------------
	Portals invert(Portals a);

};

#endif