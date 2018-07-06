#include <pt/shape/cylinder.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Cylinder::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	vec3 V = ray.dir;
	vec3 P = ray.pos - A;
	vec3 D = B - A;

	double vv = dot(V, V);
	double pp = dot(P, P);
	double dd = dot(D, D);

	double dv = dot(D, V);
	double dp = dot(D, P);
	double pv = dot(P, V);

	double a = dd * vv -  dv * dv;
	double b = 2.0 * (dd * pv - dp * dv);
	double c = dd * pp - dp * dp - r*r * dd;

	double d = b*b - 4*a*c;
	if (d >= 0) {
		d = sqrt(d);
		double t1 = (-b-d)/(2.0*a);
		double t2 = (-b+d)/(2.0*a);
		double t1p = dot(P + V*t1, D)/dd;
		double t2p = dot(P + V*t2, D)/dd;
		if (t1 > tMin && t1 < tMax && t1p >= 0 && t1p <= 1)  {
			inter.t = t1;
			inter.pos = ray.pos + ray.dir * t1;
			inter.normal = inter.pos - (A + D * t1p);
			inter.normal.normalize();
			if (dot(ray.dir, inter.normal) >= 0)
				inter.normal = -inter.normal;
			return true;
		} else
		if (t2 > tMin && t2 < tMax && t2p >= 0 && t2p <= 1)  {
			inter.t = t2;
			inter.pos = ray.pos + ray.dir * t2;
			inter.normal = inter.pos - (A + D * t2p);
			inter.normal.normalize();
			if (dot(ray.dir, inter.normal) >= 0)
				inter.normal = -inter.normal;
			return true;
		} else
			return false;
	} else
		return false;
}

};