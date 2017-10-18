#include <pt/camera/360.h>

namespace pt
{

//-----------------------------------------------------------------------------
Camera360::Camera360(Vector pos, double resolution) : Camera(pos), resolution(resolution) {}

//-----------------------------------------------------------------------------
Ray Camera360::getRay(float x, float y) {
	x -= resolution;
	y -= resolution / 2.0;
	x *= pi/resolution;
	y *= pi/resolution;
	x = -x;
	y = y + pi/2;
	Ray ray;
	ray.pos = pos;
	ray.dir = Vector(sin(y) * cos(x), sin(y) * sin(x), cos(y));
	ray.dir.normalize();
	return ray;
}

};