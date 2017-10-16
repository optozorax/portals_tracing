#include <pt/camera/orthogonal.h>

namespace pt
{

//-----------------------------------------------------------------------------
Orthogonal::Orthogonal(Vector pos, double scale, double width, double height) : Camera(pos), scale(scale), width(width), height(height) {
}

//-----------------------------------------------------------------------------
Ray Orthogonal::getRay(float x, float y) {
	// @TODO test this
	x -= width/2.0;
	y -= height/2.0;
	x *= scale;
	y *= scale;
	Ray ray;
	ray.pos = i*x + j*y + pos;
	ray.dir = k;
	return ray;
}

//-----------------------------------------------------------------------------
void Orthogonal::lookTowards(Vector toward) {
	// @TODO test this
	k = toward - pos;
	k.normalize();
	i.y = -k.x/k.y;
	i.x = 1;
	i.normalize();
	j = cross(k, i);
	j.normalize();
}

};