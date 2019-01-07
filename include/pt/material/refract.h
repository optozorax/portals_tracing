#ifndef PT_REFRACT_H
#define PT_REFRACT_H

#include <pt/object.h>

namespace pt
{

	class Refract : public Material
	{
	public:
		Refract(double refractiveIndex, double diffusion);

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;	

		double refractiveIndex;
		double diffuse;
	};

	inline Material_ptr makeRefract(double refractiveIndex, double diffusion) { return Material_ptr(new Refract(refractiveIndex, diffusion)); }

};

#endif