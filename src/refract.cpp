#include <pt/.h>

namespace pt
{

//-----------------------------------------------------------------------------
Refract::Refract(double refractiveIndex, double diffusion) {

}

//-----------------------------------------------------------------------------
ScatterType Refract::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered) const {

}

};