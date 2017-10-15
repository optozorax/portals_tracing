#include <pt/material/scatter.h>

namespace pt
{

//-----------------------------------------------------------------------------
Scatter::Scatter(Color clr) : m_clr(clr) {}

//-----------------------------------------------------------------------------
ScatterType Scatter::scatter(const Ray& ray,
					  const Intersection& inter,
					  Color& clrAbsorbtion,
					  Ray& scattered,
					  double& diffusion) const {
	clrAbsorbtion = m_clr;
	scattered.pos = inter.pos + inter.normal * 0.001;
	scattered.dir = inter.normal;
	diffusion = 1;
	return SCATTER_RAYTRACING_END;
}

};