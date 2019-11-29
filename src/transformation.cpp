#include <pt/transformation.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Transformation::intersect(const Ray& ray, 
							   Intersection& inter, 
							   double tMin, 
							   double tMax) const {
	Intersection inter1;
	Ray ray1;

	ray1.pos = transform(ray.pos);
	ray1.dir = transform(ray.pos + ray.dir) - ray1.pos;

	bool returned = m_obj->intersect(ray1, inter1, tMin, tMax);

	if (returned) {
		inter.t = inter1.t;
		inter.pos = inverse(inter1.pos);
		inter.normal = inverse(inter1.pos + inter1.normal) - inter.pos;
		inter.normal.normalize();
		return true;
	} else
		return false;
}

//-----------------------------------------------------------------------------
ScatterType Transformation::scatter(const Ray& ray,
									const Intersection& inter,
									Color& clrAbsorbtion,
									Ray& scattered,
									double& diffusion) const {
	Ray ray1;
	Intersection inter1;
	Ray scattered1;

	ray1.pos = transform(ray.pos);
	ray1.dir = transform(ray.pos + ray.dir) - ray1.pos;

	inter1.t = inter.t;
	inter1.pos = transform(inter.pos);
	inter1.normal = transform(inter.pos + inter.normal) - inter1.pos;

	ScatterType returned = m_obj->scatter(ray1, inter1, clrAbsorbtion, scattered1, diffusion);

	scattered.pos = inverse(scattered1.pos);
	scattered.dir = inverse(scattered1.pos + scattered1.dir) - scattered.pos;
	scattered.dir.normalize();
	return returned;
}

//-----------------------------------------------------------------------------
TrMove::TrMove(const vec3& diff, Object_ptr obj) : diff(diff), Transformation(obj) {}

//-----------------------------------------------------------------------------
vec3 TrMove::transform(const vec3& p) const {
	return p - diff;
}

//-----------------------------------------------------------------------------
vec3 TrMove::inverse(const vec3& p) const {
	return p + diff;
}

//-----------------------------------------------------------------------------
TrRotate::TrRotate(const vec3& angles, Object_ptr obj) : angles(angles), Transformation(obj) {}

//-----------------------------------------------------------------------------
void TrRotate::rotate2(double& x, double &y, const double& angle) const {
	double x1 = cos(angle)*x - sin(angle)*y;
	double y1 = sin(angle)*x + cos(angle)*y;
	x = x1;
	y = y1;
}

//-----------------------------------------------------------------------------
vec3 TrRotate::transform(const vec3& p) const {
	vec3 a = p;
	rotate2(a.x, a.y, angles.z);
	rotate2(a.x, a.z, angles.y);
	rotate2(a.y, a.z, angles.x);
	return a;
}

//-----------------------------------------------------------------------------
vec3 TrRotate::inverse(const vec3& p) const {
	vec3 a = p;
	rotate2(a.y, a.z, -angles.x);
	rotate2(a.x, a.z, -angles.y);
	rotate2(a.x, a.y, -angles.z);
	return a;
}

//-----------------------------------------------------------------------------
TrScale::TrScale(const vec3& scale, Object_ptr obj) : scale(scale), Transformation(obj) {}

//-----------------------------------------------------------------------------
vec3 TrScale::transform(const vec3& p) const {
	vec3 a = p;
	a.x /= scale.x;
	a.y /= scale.y;
	a.z /= scale.z;
	return a;
}

//-----------------------------------------------------------------------------
vec3 TrScale::inverse(const vec3& p) const {
	vec3 a = p;
	a.x *= scale.x;
	a.y *= scale.y;
	a.z *= scale.z;
	return a;
}

//-----------------------------------------------------------------------------
TrComplex::TrComplex(const vec3& scale, const vec3& angles, const vec3& diff, Object_ptr obj) : m_scale(scale, nullptr), m_rotate(angles, nullptr), m_move(diff, nullptr), scale(m_scale.scale), angles(m_rotate.angles), diff(m_move.diff), Transformation(obj) {}

//-----------------------------------------------------------------------------
vec3 TrComplex::transform(const vec3& p) const {
	//return m_move.transform(m_rotate.transform(m_scale.transform(p)));
	return m_scale.transform(m_rotate.transform(m_move.transform(p)));
}

//-----------------------------------------------------------------------------
vec3 TrComplex::inverse(const vec3& p) const {
	//return m_scale.inverse(m_rotate.inverse(m_move.inverse(p)));
	return m_move.inverse(m_rotate.inverse(m_scale.inverse(p)));
}

};