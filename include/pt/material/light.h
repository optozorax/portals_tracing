#ifndef PT_REFRACT_H
#define PT_REFRACT_H

#include <pt/object.h>

namespace pt
{

	class Light : public Material
	{
	public:
		Light(Color clr);

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,,
							double& diffusion) const;	
	};

};

#endif