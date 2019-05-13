#include <iostream>
#include <windows.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/shape/cone.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

using namespace pt;

//-----------------------------------------------------------------------------
void makeScene1(Scene& scene) {
	scene.add(makeSphere(vec3(0, 0, 0), 1, makeReflect(Color(0.7, 0.7, 0.7), 0)));
	const double size = 500;
	const double depth = -0.5;
	vec3 a(-size, -size, depth);
	vec3 b(-size, size, depth);
	vec3 c(size, size, depth);
	vec3 d(size, -size, depth);
	scene.add(makeTriangle(a, b, c, makeScatter(Color(0.4, 0, 0.4))));
	scene.add(makeTriangle(c, d, a, makeScatter(Color(0.4, 0, 0.6))));
	a = vec3(-5, 5, 1);
	b = vec3(-5, 6, 3);
	c = vec3(-8, 9, 5);
	scene.add(makeTriangle(a, b, c, makeScatter(Color(0.4, 0, 0.4))));
	a = vec3(0, 0, 1);
	b = vec3(-0.5, 1.2, 1);
	c = vec3(-1.2, 1.2, 1);
	scene.add(makeTriangle(a, b, c, makeScatter(pt::Color(0.4, 0.4, 0.6))));

	scene.add(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
}

//-----------------------------------------------------------------------------
void makeCam1(PerspectiveCamera& cam1, Orthogonal& cam2, Camera360& cam3, const Image& img) {
	// Инициализируем камеру с песпективной проекцией
	double beta = pi/6;
	double alpha = pi;
	vec3 pos(sin(pi/2 - beta) * cos(alpha), sin(pi/2 - beta) * sin(alpha), cos(pi/2 - beta));
	pos *= 3;
	cam1 = PerspectiveCamera(1, pi / 2.0, 0, pos, img.getWidth(), img.getHeight());
	cam1.lookAt(vec3(0));

	// Инициализируем камеру с ортогональной проекцией
	cam2 = Orthogonal(pos, 0.006, img.getWidth(), img.getHeight());
	cam2.lookTowards(vec3(0));

	// Инициализируем камеру с проекцией на сферу
	cam3 = Camera360(pos, img.getHeight());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	Image img(1000, 500);

	Scene scene;
	makeScene1(scene);
	PerspectiveCamera cam1(1, pi / 2.0, 0, vec3(0), img.getWidth(), img.getHeight());
	Orthogonal cam2(vec3(0), 0.006, img.getWidth(), img.getHeight());
	Camera360 cam3(vec3(0), img.getHeight());
	
	{	
		RayTracing ren(4, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));

		ren.assign(&cam1, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "hello_world_ray_perspective.png");

		ren.assign(&cam2, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "hello_world_ray_orthogonal.png");

		ren.assign(&cam3, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "hello_world_ray_360.png");
	}

	{
		PathTracing ren(50, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam1, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "hello_world_path_perspective.png");
	}
}