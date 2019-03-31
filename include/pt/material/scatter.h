#ifndef PT_SCATTER_H
#define PT_SCATTER_H

#include <pt/object.h>

namespace pt
{

	class Scatter : public Material
	{
	public:
		Scatter(Color clr, double k_coef = 1, double s_coef = 0);

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;	
		
		Color clr;
		double s_coef;
		double k_coef;
	};

	inline Material_ptr makeScatter(Color clr, double k_coef = 1, double s_coef = 0) { return Material_ptr(new Scatter(clr, k_coef,  s_coef)); }

};

#endif