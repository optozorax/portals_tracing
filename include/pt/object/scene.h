#ifndef PT_SCENE_H
#define PT_SCENE_H

#include <pt/object.h>

namespace pt
{

	class Scene : public Object
	{
	public:
		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

		std::vector<Object*> array;
	private:
		mutable unsigned int m_current;
	};

};

#endif