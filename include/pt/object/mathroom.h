#ifndef PT_MATHROOM_H
#define PT_MATHROOM_H

#include <string>
#include <pt/object.h>

namespace pt
{

	class MathRoom : public Object
	{
	public:
		MathRoom(void);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;
	};

	inline Object_ptr makeMathRoom(void) { return Object_ptr(new MathRoom()); }

};

#endif