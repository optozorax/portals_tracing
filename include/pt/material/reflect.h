#ifndef PT_REFLECT_H
#define PT_REFLECT_H

#include <pt/object.h>

namespace pt
{

	class Reflect : public Material
	{
	public:
		Reflect(Color clr, double diffusion);

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;	

		Color clr;
		double diffuse;
	};

	inline Material_ptr makeReflect(Color clr, double diffusion) { return Material_ptr(new Reflect(clr, diffusion)); }

};

#endif