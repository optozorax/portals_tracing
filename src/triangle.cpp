#include <pt/shape/triangle.h>

namespace pt
{

//-----------------------------------------------------------------------------
double area(double a, double b, double c) {
	double p = (a + b + c)/2.0f;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}
//-----------------------------------------------------------------------------
Triangle::Triangle(vec3 a, vec3 b, vec3 c, Material_ptr material) :
	a(a), b(b), c(c),
	Shape(material) {
	// Equation of plane, every x in plane when: (n, x) + d = 0
	normal = (cross(b-a, c-b)).normalize();
	d = -dot(normal, c);
	ab = distance(b, a);
	bc = distance(c, b);
	ac = distance(c, a);
	S = area(ab, bc, ac);				
}

//-----------------------------------------------------------------------------
bool Triangle::intersect(const Ray& ray, 
						 Intersection& inter, 
						 double tMin, 
						 double tMax) const {
	if (dot(ray.dir, normal)) {
		double t = (-d - dot(ray.pos, normal))/dot(ray.dir, normal);

		if (t > tMin && t < tMax) {
			// Position when ray intersect plane
			vec3 x = ray.pos + ray.dir * t;

			// Point in triangle <=> area of triangle = sum of inner triangles
			double xa = distance(a, x);
			double xb = distance(b, x);
			double xc = distance(c, x);

			double S1 = area(ab, xa, xb);
			double S2 = area(bc, xb, xc);
			double S3 = area(ac, xa, xc);

			if (fabs((S1 + S2 + S3)/S) < 1.0001f) {
				inter.t = t;
				if (dot(ray.dir, normal) < 0)
					inter.normal = normal;
				else
					inter.normal = -normal;
				inter.pos = x;
				return true;
			} else
				return false;
		} else
			// Ray is not in ranges
			return false;
	} else {
		// Ray is parallel plane
		return false;
	}
}

};