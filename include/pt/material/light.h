#ifndef PT_LIGHT_H
#define PT_LIGHT_H

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
							Ray& scattered,
							double& diffusion) const;	

		Color clr;
	};

};

#endif