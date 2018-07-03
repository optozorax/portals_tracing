#include <pt/material/reflect.h>

namespace pt
{

//-----------------------------------------------------------------------------
Reflect::Reflect(Color clr, double diffusion) : clr(clr), diffuse(diffusion) {
}

//-----------------------------------------------------------------------------
ScatterType Reflect::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered,
							 double& diffusion) const {
	clrAbsorbtion = clr;
	scattered.pos = inter.pos;// + inter.normal * 0.001;
	scattered.dir = ray.dir;
	reflect(scattered.dir, inter.normal);
	diffusion = diffuse;
	return SCATTER_NEXT;
}

};