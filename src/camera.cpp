#include <pt/camera.h>

namespace pt
{

//-----------------------------------------------------------------------------
Vector getRotatedVector(const Vector& pos, double r, double alpha, double beta) {
	Vector pos1(Vector(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta)));
	pos1 *= r; 
	pos1 += pos;
	return pos1;
}

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
	h = 2 * tan(viewAngle/2.0);
}

//-----------------------------------------------------------------------------
Ray PerspectiveCamera::getRay(float x, float y) const {
	x = (x-width/2.0)/height*h;
	y = (height/2.0-y)/height*h;
	Vector offset;
	double alpha = random() * 2 * pi;
	double r = random();
	offset.x = sin(alpha)*r * aperture;
	offset.y = cos(alpha)*r * aperture;
	offset = i * offset.x + j * offset.y;
	Ray ray;
	ray.dir = i*x + j*y + k - offset/focal;
	ray.pos = pos + offset;
	ray.dir.normalize();
	return ray;
}

//-----------------------------------------------------------------------------
void PerspectiveCamera::lookAt(const Vector& towards) {
	k = towards - pos;
	k.normalize();
	i = cross(Vector(0, 0, 1), k).normalize();
	j = cross(k, i).normalize();
}

};