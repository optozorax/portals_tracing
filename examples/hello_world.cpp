#include <iostream>
#include <windows.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/renderer/path-tracing.h>
#include <pt/pt2easybmp.h>

class PathRenderer2 : public pt::PathRenderer
{
public:
	PathRenderer2(int samples, 
		int maxDepth = 30,
		double maxT = 100000) : PathRenderer(samples, maxDepth, maxT) {}

	void onStartRender(void) {
		std::cout << "Render was started!" << std::endl;
		time = GetTickCount();
	}

	void onEveryLine(float percent) {
		std::cout << "\rRender time: " << (GetTickCount() - time)/1000.0 << "s; Percent: " << percent * 100 << "%";
	}

	void onEndRendering(void) {
		std::cout << "\rRender was Finished!" << std::endl;
	}

	unsigned long time;
};


int main() {
	using namespace pt;

	double beta = pi/6;
	double alpha = pi;

	Image img(500, 250);

	RayRenderer ren;
	ren.skyColor = Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(Vector(0, 0, 3), Color(0.2, 0.2, 0.2)));
	ren.luminaries.push_back(PointLightSource(Vector(0, 1, 3), Color(0.2, 0.2, 0.2)));

	Scene scene;
	scene.array.push_back(new Sphere(new Reflect(Color(0.7, 0.7, 0.7), 0)));
	//scene.array.push_back(new Sphere(new Scatter(Color(0.1, 0.1, 0.1))));
	const double size = 500;
	const double depth = -3;
	Vector a(-size, -size, depth);
	Vector b(-size, size, depth);
	Vector c(size, size, depth);
	Vector d(size, -size, depth);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(Color(0.1, 0, 0.1))));
	scene.array.push_back(new Triangle(c, d, a, new Scatter(Color(0.1, 0, 0.2))));
	a = Vector(-5, 5, 1);
	b = Vector(-5, 6, 3);
	c = Vector(-8, 9, 5);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(Color(0.1, 0, 0.1))));
	a = Vector(0, 0, 1);
	b = Vector(-0.5, 1.2, 1);
	c = Vector(-1.2, 1.2, 1);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0.1, 0.2))));

	Vector pos(sin(pi/2 - beta) * cos(alpha), sin(pi/2 - beta) * sin(alpha), cos(pi/2 - beta));
	pos *= 3;

	PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
	cam.lookAt(Vector(0));
	ren.render(cam, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "hello_world_ray_perspective.bmp");

	Camera360 cam2(pos, img.getHeight());
	ren.render(cam2, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "hello_world_ray_360.bmp");

	Orthogonal cam3(pos, 0.009 / img.getWidth() * 600.0, img.getWidth(), img.getHeight());
	cam3.lookTowards(Vector(0));
	ren.render(cam3, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "hello_world_ray_ort.bmp");

	PathRenderer2 ren2(100);
	scene.array.push_back(new Sky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
	img.clear();
	ren2.render(cam, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "hello_world_path_perspective.bmp");

	scene.array.push_back(new Sky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
	img.clear();
	ren2.render(cam2, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "hello_world_path_360.bmp");
}