#include <pt/camera/360.h>

namespace pt
{

//-----------------------------------------------------------------------------
Camera360::Camera360(vec3 pos, double resolution) : Camera(pos), resolution(resolution) {}

//-----------------------------------------------------------------------------
Ray Camera360::getRay(double x, double y, bool isDiffuse) const {
	x -= resolution;
	y -= resolution / 2.0;
	x *= pi/resolution;
	y *= pi/resolution;
	x = -x;
	y = y + pi/2;
	Ray ray;
	ray.pos = pos;
	ray.dir = vec3(sin(y) * cos(x), sin(y) * sin(x), cos(y));
	ray.dir.normalize();
	return ray;
}

};