#ifndef PT_SCATTER_H
#define PT_SCATTER_H

#include <pt/object.h>

namespace pt
{

	class Scatter : public Material
	{
	public:
		Scatter(Color clr);

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;	
		
		Color clr;
	};

	inline Material_ptr makeScatter(Color clr) { return Material_ptr(new Scatter(clr)); }

};

#endif