#include <random>
#include <mutex>
#include <math.h>
#include <pt/basics.h>

namespace pt
{

std::mt19937 generator;
std::uniform_real_distribution<double> distribution(0, 1);

//-----------------------------------------------------------------------------
std::mutex m;
double random(void) {
	m.lock();
	double result = distribution(generator);
	m.unlock();
	return result;
}

//-----------------------------------------------------------------------------
Color Color::operator/=(double k) {
	r /= k;
	g /= k;
	b /= k;
	a /= k;
	return *this;
}

//-----------------------------------------------------------------------------
Color overlay(const Color& upper, const Color& lower) {
	Color out;
	out.a = upper.a + lower.a * (1 - upper.a);
	out.r = (upper.r * upper.a + lower.r * lower.a * (1 - upper.a))/out.a;
	out.g = (upper.g * upper.a + lower.g * lower.a * (1 - upper.a))/out.a;
	out.b = (upper.b * upper.a + lower.b * lower.a * (1 - upper.a))/out.a;
	return out;
}

//-----------------------------------------------------------------------------
Color Color::operator*(const Color& a) const {
	Color clr = *this;
	clr.a = clr.a + a.a*(1 - clr.a);
	clr.r *= a.r;
	clr.g *= a.g;
	clr.b *= a.b;
	return clr;
}

//-----------------------------------------------------------------------------
Color Color::operator+=(const Color& clr) {
	a += clr.a;
	r += clr.r;
	g += clr.g;
	b += clr.b;
	return *this;
}

//-----------------------------------------------------------------------------
Color Color::operator*(double a) const {
	Color clr = *this;
	clr.r *= a;
	clr.g *= a;
	clr.b *= a;
	return clr;
}

//-----------------------------------------------------------------------------
Color Color::sqrt(void) {
	r = ::sqrt(r);
	g = ::sqrt(g);
	b = ::sqrt(b);
	return *this;
}

//-----------------------------------------------------------------------------
void reflect(vec3& ray, const vec3& normal) {
	ray = ray - normal * dot(ray, normal) / dot(normal, normal) * 2;
}

//-----------------------------------------------------------------------------
bool refract(vec3& ray, const vec3& normal, double r) {
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
vec3 randomSphere(void) {
	/*double alpha = random() * 2 * pi;
	double beta = random() * 2 * pi;
	vec3 r;
	r.x = sin(alpha) * cos(beta);
	r.y = sin(alpha) * sin(beta);
	r.z = cos(beta);
	return r;*/
	// WTF?
	vec3 v;
	do {
		v = vec3(random(), random(), random()) * 2 - vec3(1);
	} while(v.length() > 1);
	return v;
}

};