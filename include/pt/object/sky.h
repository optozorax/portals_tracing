#ifndef PT_SKY_H
#define PT_SKY_H

#include <pt/object.h>

namespace pt
{

	class Sky : public Object
	{
	public:
		Sky(Color clr1, Color clr2);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

		Color clr1;
		Color clr2;
	};

};

#endif