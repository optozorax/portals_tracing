#include <pt/material/refract.h>

namespace pt
{

//-----------------------------------------------------------------------------
Refract::Refract(double refractiveIndex, double diffusion) : refractiveIndex(refractiveIndex), diffusion(diffusion) {

}

//-----------------------------------------------------------------------------
ScatterType Refract::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered,
							 double& diffusion) const {
	// @TODO make this
}

};