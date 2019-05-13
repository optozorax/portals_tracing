#include <pt/camera/orthogonal.h>

namespace pt
{

//-----------------------------------------------------------------------------
Orthogonal::Orthogonal(vec3 pos, double scale, double width, double height) : Camera(pos), scale(scale), width(width), height(height) {
}

//-----------------------------------------------------------------------------
Ray Orthogonal::getRay(double x, double y, bool isDiffuse) const {
	x -= width/2.0;
	y -= height/2.0;
	x *= scale;
	y *= scale;
	Ray ray;
	ray.pos = -i*x - j*y + pos;
	ray.dir = k;
	return ray;
}

//-----------------------------------------------------------------------------
void Orthogonal::lookTowards(vec3 toward) {
	k = toward - pos;
	k.normalize();
	if (!(k.x == 0 && k.y == 0)) {
		i.x = -k.y;
		i.y = k.x;
		i.normalize();
	}
	j = cross(k, i);
	i = -i;
	j.normalize();
}

};