#include <pt/transformation.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Transformation::intersect(const Ray& ray, 
							   Intersection& inter, 
							   double tMin, 
							   double tMax) const {

}

//-----------------------------------------------------------------------------
ScatterType Transformation::scatter(const Ray& ray,
									const Intersection& inter,
									Color& clrAbsorbtion,
									Ray& scattered,
									double& diffusion) const {

}

};