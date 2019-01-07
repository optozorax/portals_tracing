#ifndef PT_SPHERE_H
#define PT_SPHERE_H

#include <pt/object.h>

namespace pt
{

	class Sphere : public Shape
	{
	public:
		Sphere(vec3 a, double r, Material_ptr material) : Shape(material), A(a), r(r) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		vec3 A;
		double r;
	};

	inline Object_ptr makeSphere(vec3 a, double r, Material_ptr material) { return Object_ptr(new Sphere(a, r, material)); }

};

#endif