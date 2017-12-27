#ifndef PT_CYLINDER_H
#define PT_CYLINDER_H

#include <pt/object.h>

namespace pt
{

	class Cylinder : public Shape
	{
	public:
		Cylinder(Vector a, Vector b, double r, Material* material) : Shape(material), A(a), B(b), r(r) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		Vector A, B;
		double r;
	};

};

#endif