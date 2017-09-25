#ifndef PT_TRIANGLE_H
#define PT_TRIANGLE_H

#include <pt/object.h>

namespace pt
{

	class Triangle : public Shape
	{
	public:
		Triangle(Vector a, Vector b, Vector c, Material* material) :
			a(a), b(b), c(c),
			Shape(material) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   float tMin, 
					   float tMax) const;

		Vector a, b, c;
	};

};

#endif