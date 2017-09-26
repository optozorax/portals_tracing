#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;

	Scene scene;
	scene.array.push_back(new Sphere(Vector(0, -1000, 0), 1000, new Scatter(Color(0.1, 0.1, 0.1))));
	scene.array.push_back(new Sphere(Vector(0, 1, 0), 1, new Scatter(Color(0.5, 0, 0.5))));

	PerspectiveCamera cam(1, pi/2.0, Vector(3, 3, 3));
	cam.lookAt(Vector(0, 1, 0));

	RayRenderer ren;
	PointLightSource p;
	ren.luminaries.push_back(PointLightSource(Vector(3, 3, -3), Color(1, 1, 1)));

	Image img(1024, 768);

	ren.render(cam, img, scene);

	img.colorCorrection();

	saveAsBmp(img, "hello_world.bmp");
}