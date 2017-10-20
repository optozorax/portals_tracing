#include <pt/shape/cylinder.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Cylinder::intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const {
	double t, tcir, tcyl;
	bool isCircle = false;

	Vector circNorm;
	if (ray.dir.z != 0) {
		double x3, y3, x4, y4;
		double t3, t4;

		bool isUpperCircle = false;
		bool isLowerCircle = false;
		t3 = (0.5 - ray.pos.z) / ray.dir.z;
		t4 = (-0.5 - ray.pos.z) / ray.dir.z;
		x3 = ray.pos.x + ray.dir.x * t3;
		y3 = ray.pos.y + ray.dir.y * t3;
		x4 = ray.pos.x + ray.dir.x * t4;
		y4 = ray.pos.y + ray.dir.y * t4;

		isUpperCircle = (x3 * x3 + y3 * y3) <= 0.25;
		isLowerCircle = (x4 * x4 + y4 * y4) <= 0.25;
		isUpperCircle &= t3 >= tMin && t3 <= tMax;
		isLowerCircle &= t4 >= tMin && t4 <= tMax;

		if (isUpperCircle && isLowerCircle) {
			isCircle = true;
			if (t3 < t4) {
				tcir = t3;
				circNorm = Vector(0, 0, 1);
			} else {
				tcir = t4;
				circNorm = Vector(0, 0, -1);
			}
		} else if (isLowerCircle) {
			tcir = t4;
			circNorm = Vector(0, 0, -1);
			isCircle = true;
		} else if (isUpperCircle) {
			tcir = t3;
			circNorm = Vector(0, 0, 1);
			isCircle = true;
		} else
			isCircle = false;
	}

	double a = ray.dir.x * ray.dir.x + ray.dir.y * ray.dir.y;
	double b = 2*(ray.dir.x * ray.pos.x + ray.dir.y * ray.pos.y);
	double c = ray.pos.x * ray.pos.x + ray.pos.y * ray.pos.y - 0.25;
	double d = b*b - 4*a*c;

	bool isCylinder = d > 0;
	if (isCylinder) {
		double t1, t2;
		if (fabs(a) > 0.00001) {
			t1 = (-b + sqrt(d)) / (2*a);
			t2 = (-b - sqrt(d)) / (2*a);
		} else {
			t2 = t1 = -c/b;
		}

		if (t1 < t2 && t1 >= tMin && t1 <= tMax)
			tcyl = t1;
		else if (t2 >= tMin && t2 <= tMax)
			tcyl = t2;
		else
			isCylinder = false;

		if (isCylinder) {
			double z = ray.pos.z + ray.dir.z * tcyl;
			if (fabs(z) > 0.5)
				isCylinder = false;
		}
	}

	bool returned = false;
	if (isCylinder && isCircle) {
		returned = true;
		if (tcyl < tcir)
			t = tcyl;
		else
			t = tcir;
	} else if (isCylinder) {
		t = tcyl;
		returned = true;
	} else if (isCircle) {
		t = tcir;
		returned = true;
	} else
		returned = false;

	if (returned) {
		inter.t = t;
		inter.pos = ray.pos + ray.dir * t;
		if (isCylinder)
			inter.normal = Vector(inter.pos.x, inter.pos.y, 0);
		else 
			inter.normal = circNorm;
		inter.normal.normalize();
		return true;	
	} else
		return false;
}

};