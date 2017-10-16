#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/material/scatter.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;

	double beta = pi/12;
	double alpha = pi;

	Image img(500, 250);
	PerspectiveCamera cam(1, pi/2.0, 0, Vector(0), img.getWidth(), img.getHeight());

	RayRenderer ren;
	ren.skyColor = Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(Vector(0, 0, 3), Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(Vector(0, 1, 3), Color(1.2, 1.2, 1.2)));

	Scene scene;
	scene.array.push_back(new Sphere(new Scatter(Color(0.1, 0.1, 0.1))));
	const double size = 500;
	const double depth = -3;
	Vector a(-size, -size, depth);
	Vector b(-size, size, depth);
	Vector c(size, size, depth);
	Vector d(size, -size, depth);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(Color(0.1, 0, 0.1))));
	scene.array.push_back(new Triangle(c, d, a, new Scatter(Color(0.1, 0, 0.1))));

	Vector pos(sin(pi/2 - beta) * cos(alpha), sin(pi/2 - beta) * sin(alpha), cos(pi/2 - beta));
	pos *= 3;
	cam.pos = pos;
	cam.lookAt(Vector(0, 0, 0));

	ren.render(cam, img, scene);
	img.colorCorrection();
	saveAsBmp("hello_world.bmp");
}