#ifndef PT_TRIANGLE_H
#define PT_TRIANGLE_H

#include <pt/object.h>

namespace pt
{

	class Triangle : public Shape
	{
	public:
		Triangle(vec3 a, vec3 b, vec3 c, Material* material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		vec3 a, b, c;
	private:
		vec3 normal;
		double d;
		double S;
		double ab, bc, ac;
	};

};

#endif