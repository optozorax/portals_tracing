#include "portals.h"

namespace pt
{

//-----------------------------------------------------------------------------
Portals::Portals(Vector a, Vector b, Vector c,
				 Vector a1, Vector b1, Vector c1,
				 Color backside) {
	assign(a, b, c, a1, b1, c1, backside);
}

//-----------------------------------------------------------------------------
Portals::~Portals() {
	delete t1;
	delete t2;
	delete back;
}

//-----------------------------------------------------------------------------
void calculateIjkFromAbc(const Vector& a, const Vector& b, const Vector& c,
						 Vector& i, Vector& j, Vector& k) {
	i = (b - a).normalize();
	j = c - a;
	j = (j - i*dot(j, i)).normalize();
	k = cross(j, i).normalize();
}

//-----------------------------------------------------------------------------
void Portals::assign(Vector _a, Vector b, Vector c,
					 Vector _a1, Vector b1, Vector c1,
					 Color backside) {
	a = _a;
	a1 = _a1;
	calculateIjkFromAbc(a, b, c, i, j, k);
	calculateIjkFromAbc(a1, b1, c1, i1, j1, k1);
	t1 = new Triangle(a, b, c, nullptr);
	t2 = new Triangle(a1, b1, c1, nullptr);
	back = new Scatter(backside);
}

//-----------------------------------------------------------------------------
bool Portals::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	Intersection inter1, inter2;
	bool isFirst = t1->intersect(ray, inter1, tMin, tMax);
	bool isSecond = t2->intersect(ray, inter2, tMin, tMax);

	isFirst &= inter1.t >= tMin && inter1.t <= tMax;
	isSecond &= inter2.t >= tMin && inter2.t <= tMax;
	bool firstLessSecond = inter1.t < inter2.t;

	if (isFirst && isSecond && firstLessSecond) {
		currentTriangle = 1;
		inter = inter1;
		return true;
	}

	if (isFirst && isSecond && !firstLessSecond) {
		currentTriangle = 2;
		inter = inter2;
		return true;
	}	

	if (isFirst) {
		currentTriangle = 1;
		inter = inter1;
		return true;
	}

	if (isSecond) {
		currentTriangle = 2;
		inter = inter2;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
ScatterType Portals::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered,
							 double& diffusion) const {
	if (currentTriangle == 1)
		if (dot(inter.normal, k) > 0)
			return back->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
	if (currentTriangle == 2)
		if (dot(inter.normal, k1) < 0)
			return back->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);

	Vector p1;
	Vector d1;
	if (currentTriangle == 1) {
		double x, y, z;
		Vector p = inter.pos;
		Vector d = ray.dir;

		x = dot(p-a, i);
		y = dot(p-a, j);
		p1 = a1 + i1*x + j1*y;

		Vector r1 = p-a+d - k*dot(p-a+d, k);
		x = dot(r1, i);
		y = dot(r1, j);
		z = (p-a+d-r1).getLength();
		d1 = a1 + i1*x + j1*y + k1*z - p1;
		d1.normalize();

		p1 = p1 + k1 * 0.001;
	} else {
		double x, y, z;
		Vector p = inter.pos;
		Vector d = ray.dir;
		
		x = dot(p-a1, i1);
		y = dot(p-a1, j1);
		p1 = a + i*x + j*y;

		Vector r1 = p-a1+d - k1*dot(p-a1+d, k1);
		x = dot(r1, i1);
		y = dot(r1, j1);
		z = -(p-a1+d-r1).getLength();
		d1 = a + i*x + j*y + k*z - p1;
		d1.normalize();

		p1 = p1 - k * 0.001;
	}

	clrAbsorbtion = Color(1, 1, 1, 1);
	scattered.pos = p1;
	scattered.dir = d1.normalize();
	diffusion = 0;
	return SCATTER_NEXT;
}

}