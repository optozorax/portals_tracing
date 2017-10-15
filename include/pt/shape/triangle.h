#ifndef PT_TRIANGLE_H
#define PT_TRIANGLE_H

#include <pt/object.h>

namespace pt
{

	class Triangle : public Shape
	{
	public:
		Triangle(Vector a, Vector b, Vector c, Material* material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		Vector a, b, c;
	private:
		Vector normal;
		double d;
		double S;
		double ab, bc, ac;
	};

};

#endif