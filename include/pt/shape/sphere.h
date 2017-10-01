#ifndef PT_SPHERE_H
#define PT_SPHERE_H

#include <pt/object.h>

namespace pt
{

	class Sphere : public Shape
	{
	public:
		Sphere(Material* material) : Shape(material) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   float tMin, 
					   float tMax) const;
	};

};

#endif