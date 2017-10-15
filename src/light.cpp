#include <pt/material/light.h>

namespace pt
{

//-----------------------------------------------------------------------------
Light::Light(Color clr) : clr(clr) {

}

//-----------------------------------------------------------------------------
ScatterType Light::scatter(const Ray& ray,
						   const Intersection& inter,
						   Color& clrAbsorbtion,
						   Ray& scattered,
						   double& diffusion) const {
	clrAbsorbtion = clr;
	diffusion = 0;
	return SCATTER_END;
}

};