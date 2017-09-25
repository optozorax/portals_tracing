#ifndef PT_SCATTER_H
#define PT_SCATTER_H

#include <pt/object.h>

namespace pt
{

	class Scatter : public Material
	{
	public:
		Scatter(Color clr);

		bool scatter(const Ray& ray,
					 const Intersection& inter,
					 Color& clrAbsorbtion,
					 Ray& scattered) const;	
	};

};

#endif