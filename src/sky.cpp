#include <pt/.h>

namespace pt
{

//-----------------------------------------------------------------------------
Sky::Sky(Color clr1, Color clr2) {
	
}

//-----------------------------------------------------------------------------
bool Sky::intersect(const Ray& ray, 
					Intersection& inter, 
					float tMin, 
					float tMax) const {

}

//-----------------------------------------------------------------------------
bool Sky::scatter(const Ray& ray,
				  const Intersection& inter,
				  Color& clrAbsorbtion,
				  Ray& scattered) const {

}

};