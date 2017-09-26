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
					   float tMin, 
					   float tMax) const;
		bool scatter(const Ray& ray,
					 const Intersection& inter,
					 Color& clrAbsorbtion,
					 Ray& scattered) const;

		std::vector<Object*> array;
	};

};

#endif