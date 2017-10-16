#include <pt/transformation.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Transformation::intersect(const Ray& ray, 
							   Intersection& inter, 
							   double tMin, 
							   double tMax) const {
	// @TODO make this
	return false;
}

//-----------------------------------------------------------------------------
ScatterType Transformation::scatter(const Ray& ray,
									const Intersection& inter,
									Color& clrAbsorbtion,
									Ray& scattered,
									double& diffusion) const {
	// @TODO make this
	return SCATTER_END;
}

};