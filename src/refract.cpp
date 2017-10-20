#include <pt/material/refract.h>

namespace pt
{

//-----------------------------------------------------------------------------
Refract::Refract(double refractiveIndex, double diffusion) : refractiveIndex(refractiveIndex), m_diffusion(diffusion) {

}

//-----------------------------------------------------------------------------
ScatterType Refract::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered,
							 double& diffusion) const {
	double ri;
	Vector normal;

	if (dot(ray.dir, inter.normal) > 0) {
		// Out of object
		ri = refractiveIndex;
		normal = -inter.normal;
	} else {
		// In object
		ri = 1 / refractiveIndex;
		normal = inter.normal;
	}

	scattered.dir = ray.dir;
	if (!refract(scattered.dir, normal, ri)) {
		scattered.dir = ray.dir;
		reflect(scattered.dir, normal);
	}

	scattered.pos = inter.pos + scattered.dir * 0.001;
	diffusion = m_diffusion;
	clrAbsorbtion = Color(1, 1, 1, 1);

	return SCATTER_NEXT;
}

};