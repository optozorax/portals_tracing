#ifndef PT_PORTALS_H
#define PT_PORTALS_H

#include <pt/vector.h>
#include <pt/object.h>
#include <pt/material/scatter.h>
#include <pt/shape/triangle.h>

namespace pt
{

	class Portals : public Object
	{
	public:
		Portals(Vector a, Vector b, Vector c,
				Vector a1, Vector b1, Vector c1,
				Color backside);
		~Portals();

		void assign(Vector a, Vector b, Vector c,
					Vector a1, Vector b1, Vector c1,
					Color backside);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;
	private:
		Vector i, j, k;
		Vector i1, j1, k1;
		Vector a, a1;
		Triangle *t1, *t2;
		Scatter *back;
		mutable int currentTriangle;
	};

};

#endif