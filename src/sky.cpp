#include <pt/.h>

namespace pt
{

//-----------------------------------------------------------------------------
Sky::Sky(Color clr1, Color clr2) : m_clr1(clr1), m_clr2(clr2) {}

//-----------------------------------------------------------------------------
bool Sky::intersect(const Ray& ray, 
					Intersection& inter, 
					float tMin, 
					float tMax) const {
	inter.t = tMax;
	inter.pos = ray.pos + ray.dir * inter.t;
	inter.normal = ray.dir;
}

//-----------------------------------------------------------------------------
ScatterType Sky::scatter(const Ray& ray,
						 const Intersection& inter,
						 Color& clrAbsorbtion,
						 Ray& scattered,
						 double& diffusion) const {
	float k = (in.direction.identity().y + 1) * 0.5f;
	clrAbsorbtion.r = clr1.r * (1 - k) + clr2.r * k;
	clrAbsorbtion.g = clr1.g * (1 - k) + clr2.g * k;
	clrAbsorbtion.b = clr1.b * (1 - k) + clr2.b * k;
	clrAbsorbtion.a = 1;
	diffusion = 0;
	return SCATTER_END;
}

};