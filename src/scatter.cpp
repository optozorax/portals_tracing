#include <pt/material/scatter.h>

namespace pt
{

//-----------------------------------------------------------------------------
Scatter::Scatter(Color clr, double k_coef, double s_coef) : clr(clr), k_coef(k_coef), s_coef(s_coef) {}

//-----------------------------------------------------------------------------
ScatterType Scatter::scatter(const Ray& ray,
					  const Intersection& inter,
					  Color& clrAbsorbtion,
					  Ray& scattered,
					  double& diffusion) const {
	clrAbsorbtion = clr*(k_coef + s_coef*dot(vec3(-ray.dir).normalize(), vec3(inter.normal).normalize()));
	scattered.pos = inter.pos;
	scattered.dir = inter.normal;
	diffusion = 1;
	return SCATTER_RAYTRACING_END;
}

};