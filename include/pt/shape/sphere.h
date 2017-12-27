#ifndef PT_SPHERE_H
#define PT_SPHERE_H

#include <pt/object.h>

namespace pt
{

	class Sphere : public Shape
	{
	public:
		Sphere(Vector a, double r, Material* material) : Shape(material), A(a), r(r) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		Vector A;
		double r;
	};

};

#endif