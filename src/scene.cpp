#include <pt/.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Scene::intersect(const Ray& ray, 
					  Intersection& inter, 
					  float tMin, 
					  float tMax) const {

}

//-----------------------------------------------------------------------------
bool Scene::scatter(const Ray& ray,
					const Intersection& inter,
					Color& clrAbsorbtion,
					Ray& scattered) const {

}

};