#ifndef PT_CYLINDER_H
#define PT_CYLINDER_H

#include <pt/object.h>

namespace pt
{

	class Cylinder : public Shape
	{
	public:
		Cylinder(vec3 a, vec3 b, double r, Material* material) : Shape(material), A(a), B(b), r(r) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		vec3 A, B;
		double r;
	};

};

#endif