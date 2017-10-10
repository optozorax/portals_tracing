#include <pt/.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Transformation::intersect(const Ray& ray, 
							   Intersection& inter, 
							   float tMin, 
							   float tMax) const {

}

//-----------------------------------------------------------------------------
bool Transformation::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered) const {

}

};