#include <math.h>
#include <pt/basics.h>
#include <pt/vector.h>

namespace pt
{

//-----------------------------------------------------------------------------
Vector cross(const Vector& a, const Vector& b) {
	return Vector(a.y * b.z - a.z * b.y, b.x * a.z - a.x * b.z, a.x * b.y - a.y * b.x);
}

//-----------------------------------------------------------------------------
double dot(const Vector& a, const Vector& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//-----------------------------------------------------------------------------
void reflect(Vector& ray, const Vector& normal) {
	ray = ray - normal * dot(ray, normal) / dot(normal, normal) * 2;
}

//-----------------------------------------------------------------------------
bool refract(Vector& ray, const Vector& normal, double r) {
	ray.normalize();
	double c =  -dot(normal, ray.normalize());
	double d = 1 - r*r*(1 - c*c);
	if (d > 0) {
		ray = ray*r + normal*(r*c - sqrt(d));
		return true;
	} else 
		return false;
}

//-----------------------------------------------------------------------------
Vector randomSphere(void) {
	double alpha = random() * 2 * pi;
	double beta = random() * 2 * pi;
	Vector r;
	r.x = sin(alpha) * cos(beta);
	r.y = sin(alpha) * sin(beta);
	r.z = cos(beta);
	return r;
}

};