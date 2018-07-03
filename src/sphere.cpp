#include <pt/shape/sphere.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Sphere::intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const {
	Vector V = ray.dir;
	Vector P = ray.pos - A;

	double vv = dot(V, V);
	double pp = dot(P, P);
	double pv = dot(P, V);

	double a = vv;
	double b = 2.0 * pv;
	double c = pp - r*r;

	double d = b*b - 4.0*a*c;
	if (d >= 0) {
		d = sqrt(d);
		double t1 = (-b-d)/(2.0*a);
		double t2 = (-b+d)/(2.0*a);
		if (t1 > tMin && t1 < tMax) {
			inter.t = t1;
			inter.pos = ray.pos + ray.dir * t1;
			inter.normal = inter.pos - A;
			inter.normal.normalize();
			//if (dot(ray.dir, inter.normal) >= 0)
				//inter.normal = -inter.normal;
			return true;
		} else
		if (t2 > tMin && t2 < tMax) {
			inter.t = t2;
			inter.pos = ray.pos + ray.dir * t2;
			inter.normal = inter.pos - A;
			inter.normal.normalize();
			//if (dot(ray.dir, inter.normal) >= 0)
				//inter.normal = -inter.normal;
			return true;
		} else
			return false;
	} else
		return false;
}

};