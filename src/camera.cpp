#include <pt/camera.h>

namespace pt
{

//-----------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(double focal, double viewAngle, double aperture, Vector pos, double width, double height) : focal(focal), aperture(aperture), Camera(pos), width(width), height(height) {
	h = 2*focal * sin(viewAngle/2.0) / cos(viewAngle/2.0);
}

//-----------------------------------------------------------------------------
Ray PerspectiveCamera::getRay(float x, float y) {
	// @TODO test this
	x -= width/2.0;
	y -= height/2.0;
	x /= width/2.0;
	y /= width/2.0;
	x *= h/2.0;
	y *= h/2.0;
	Ray ray;
	ray.dir = (pos + i*x + j*y + k*focal) - pos;
	ray.pos = pos;
	ray.dir.normalize();
}

//-----------------------------------------------------------------------------
void PerspectiveCamera::lookAt(const Vector& towards) {
	// @TODO test this
	k = towards - pos;
	k.normalize();
	i.y = -k.x/k.y;
	i.x = 1;
	i.normalize();
	j = cross(k, i);
	j.normalize();
}

};