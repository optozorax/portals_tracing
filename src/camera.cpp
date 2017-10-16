#include <pt/camera.h>

namespace pt
{

//-----------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(double focal, double viewAngle, double aperture, Vector pos, double width, double height) : Camera(pos) {
	assign(focal, viewAngle, aperture, pos, width, height);
}

void PerspectiveCamera::assign(double focal1, double viewAngle1, double aperture1, Vector pos1, double width1, double height1) {
	viewAngle = viewAngle1;
	focal = focal1;
	aperture = aperture1;
	pos = pos1;
	width = width1;
	height = height1;
	h = 2 * focal * sin(viewAngle/2.0) / cos(viewAngle/2.0);
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
	x = -x;
	y = -y;
	Ray ray;
	ray.dir = (pos + i*x + j*y + k*focal) - pos;
	ray.pos = pos;
	ray.dir.normalize();
	return ray;
}

//-----------------------------------------------------------------------------
void PerspectiveCamera::lookAt(const Vector& towards) {
	k = towards - pos;
	k.normalize();
	if (!(k.x == 0 && k.y == 0)) {
		i.x = -k.y;
		i.y = k.x;
		i.normalize();
	}
	j = cross(k, i);
	j.normalize();
}

};