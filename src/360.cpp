#include <pt/camera/360.h>

namespace pt
{

//-----------------------------------------------------------------------------
Camera360::Camera360(Vector pos, double resolution) : pos(pos), resolution(resolution) {}

//-----------------------------------------------------------------------------
Ray Camera360::getRay(float x, float y) {
	x -= resolution/2.0;
	y -= resolution;
	x *= pi/resolution;
	y *= pi/resolution;
	Ray ray;
	ray.pos = pos;
	ray.dir = Vector((sin(y) * cos(x), sin(y) * sin(x), cos(y)));
	return ray;
}

};