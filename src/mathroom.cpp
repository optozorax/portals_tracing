#include <cmath>
#include <pt/object/mathroom.h>

namespace pt
{

//-----------------------------------------------------------------------------
MathRoom::MathRoom() {
}

//-----------------------------------------------------------------------------
bool MathRoom::intersect(const Ray& ray, 
					Intersection& inter, 
					double tMin, 
					double tMax) const {
	double t;
	if (ray.dir.z == 0) {
		t = tMax;
		goto to_sphere;
	}
	t = -ray.pos.z/ray.dir.z;
	if (tMin < t && t < tMax) {
		// Попали на плоскость пола
		inter.t = t;
		inter.normal = vec3(0, 0, 1);
		inter.pos = ray.pos + ray.dir * inter.t;

		x = inter.pos.x;
		y = inter.pos.y;
		isPlane = true;
	} else {
		// Попали на полусферу неба
		to_sphere:
		alpha = std::atan2(ray.dir.y, ray.dir.x);
		beta = std::atan2(std::sqrt(ray.dir.x*ray.dir.x + ray.dir.y*ray.dir.y), ray.dir.z);
		isPlane = false;

		inter.t = tMax;
		inter.pos = ray.pos + ray.dir * inter.t;
		inter.normal = ray.dir;
	}
	return true;
}

//-----------------------------------------------------------------------------
ScatterType MathRoom::scatter(const Ray& ray,
						 const Intersection& inter,
						 Color& clrAbsorbtion,
						 Ray& scattered,
						 double& diffusion) const {
	if (isPlane) {
		const double axisWidth = 0.035;
		const double gridWidth = 0.01;
		const Color axisColor(0, 0, 0);
		const Color gridColor(0.5, 0.5, 0.5);
		const Color planeColor(0.9, 0.9, 0.9);
		const double gridSize = 1;

		if (fabs(x) < axisWidth || fabs(y) < axisWidth) {
			clrAbsorbtion = axisColor;
		} else {
			double integral = 0;
			x /= gridSize;
			y /= gridSize;
			x = fabs(modf(x, &integral));
			if (x > 0.5)
				x = 1 - x;
			y = fabs(modf(y, &integral));
			if (y > 0.5)
				y = 1 - y;
			if (fabs(x) < gridWidth || fabs(y) < gridWidth) {
				clrAbsorbtion = gridColor;
			} else {
				clrAbsorbtion = planeColor;
			}
		}

		scattered.pos = inter.pos;
		scattered.dir = inter.normal;
		diffusion = 0;
		return SCATTER_RAYTRACING_END;
	} else {
		const Color lower(0, 0.5, 0.5);
		const Color upper(0.8, 0.8, 0.8);
		double between = 1 - beta/(pi/2.0);

		clrAbsorbtion.r = upper.r*between + lower.r*(1-between);
		clrAbsorbtion.g = upper.g*between + lower.g*(1-between);
		clrAbsorbtion.b = upper.b*between + lower.b*(1-between);
		clrAbsorbtion.a = upper.a*between + lower.a*(1-between);
		diffusion = 0;
		return SCATTER_END;
	}
}

};