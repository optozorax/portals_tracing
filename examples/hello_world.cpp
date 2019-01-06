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
#include <pt/renderer1.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;

	double beta = pi/6;
	double alpha = pi;

	Image img(1000, 500);

	Scene scene;
	scene.array.push_back(new Sphere(vec3(0, 0, 0), 1, new Reflect(Color(0.7, 0.7, 0.7), 0)));
	const double size = 500;
	const double depth = -0.5;
	vec3 a(-size, -size, depth);
	vec3 b(-size, size, depth);
	vec3 c(size, size, depth);
	vec3 d(size, -size, depth);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(Color(0.4, 0, 0.4))));
	scene.array.push_back(new Triangle(c, d, a, new Scatter(Color(0.4, 0, 0.6))));
	a = vec3(-5, 5, 1);
	b = vec3(-5, 6, 3);
	c = vec3(-8, 9, 5);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(Color(0.4, 0, 0.4))));
	a = vec3(0, 0, 1);
	b = vec3(-0.5, 1.2, 1);
	c = vec3(-1.2, 1.2, 1);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.4, 0.4, 0.6))));

	vec3 pos(sin(pi/2 - beta) * cos(alpha), sin(pi/2 - beta) * sin(alpha), cos(pi/2 - beta));
	pos *= 3;

	//scene.array.push_back(new Cone({1, 1, 0}, {1, 1, 1.01}, 0.2, new Scatter(pt::Color(0.4, 0.4, 0.6))));

	scene.array.push_back(new Sky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
	
	{
		PerspectiveCamera cam(1, pi / 2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(vec3(0));
		RayTracing ren(4, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsBmp(img, "hello_world_ray_perspective1.bmp");
	}

	{
		PerspectiveCamera cam(1, pi / 2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(vec3(0));
		PathTracing ren(50, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsBmp(img, "hello_world_path_perspective1.bmp");
	}
}