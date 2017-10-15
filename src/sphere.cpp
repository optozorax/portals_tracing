#include <pt/shape/sphere.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Sphere::intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const {
	double a = dot(ray.dir, ray.dir);
	double b = 2*dot(ray.dir, ray.pos);
	double c = dot(ray.pos, ray.pos);
	double d = b*b - 4*a*c;
	if (d < 0)
		return false;

	double t1 = (-b + sqrt(d)) / (2*a);
	double t2 = (-b - sqrt(d)) / (2*a);
	double t;

	if (t1 < t2 && t1 >= tMin && t1 <= tMax)
		t = t1;
	else
		if (t2 >= tMin && t2 <= tMax)
			t = t2;
		else
			return false;

	inter.t = t;
	inter.pos = ray.pos + ray.dir * t;
	inter.normal = inter.pos;
	return true;	
}

};