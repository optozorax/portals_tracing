#ifndef PT_CYLINDER_H
#define PT_CYLINDER_H

#include <pt/object.h>

namespace pt
{

	class Cylinder : public Shape
	{
	public:
		Cylinder(Material* material) : Shape(material) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	};

};

#endif