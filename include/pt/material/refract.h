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
		double diffusion;
	};

};

#endif