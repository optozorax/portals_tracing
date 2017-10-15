#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/sphere.h>
#include <pt/material/scatter.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;

	Image img(1024, 768);

	Scene scene;
	scene.array.push_back(new Sphere(new Scatter(Color(0.1, 0.1, 0.1))));
	scene.array.push_back(new Sphere(new Scatter(Color(0.5, 0, 0.5))));

	PerspectiveCamera cam(1, pi/2.0, 0, Vector(3, 3, 3), img.getWidth(), img.getHeight());
	cam.lookAt(Vector(0, 1, 0));

	RayRenderer ren;
	ren.luminaries.push_back(PointLightSource(Vector(3, 3, -3), Color(1, 1, 1)));

	ren.render(cam, img, scene);

	img.colorCorrection();

	saveAsBmp(img, "hello_world.bmp");
}