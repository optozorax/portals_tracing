#include <iostream>
#include <random>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/material/refract.h>
#include <pt/renderer1.h>
#include <pt/pt2easybmp.h>

using namespace pt;

float myrandom() {
	static std::mt19937 generator(1);
	static std::uniform_real_distribution<float> distribution(0, 1);
	return distribution(generator);
}

void initScene(Scene& scene) {
	auto& array = scene.array;
	array.push_back(new Sky(Color(1, 1, 1), Color(0.5, 0.7, 1).sqrt()));

	for(int i = -11; i < 11; ++i) {
		for(int j = -11; j < 11; ++j) {
			vec3 c(i + 0.9 * myrandom(), j + 0.9 * myrandom(), 0.2);
			double p = myrandom();
			if(p < 0.8) {
				array.push_back(new Sphere(c, 0.2, new Scatter(Color(myrandom(), myrandom(), myrandom()).sqrt())));
			} else if(p < 0.95) {
				double r = 0.5 * (1 + myrandom());
				double g = 0.5 * (1 + myrandom());
				double b = 0.5 * (1 + myrandom());
				array.push_back(new Sphere(c, 0.2, new Reflect(Color(r, g, b).sqrt(), 0.5 * myrandom())));
			} else {
				array.push_back(new Sphere(c, 0.2, new Refract(1.5, 0)));
			}
		}
	}
	array.push_back(new Sphere(vec3(0, 0, 1), 1, new Refract(1.5, 0)));
	array.push_back(new Sphere(vec3(-4, 0, 1), 1, new Scatter(Color(0.4, 0.2, 0.1).sqrt())));
	array.push_back(new Sphere(vec3(4, 0, 1), 1, new Reflect(Color(0.7, 0.6, 0.5).sqrt(), 0)));
	array.push_back(new Sphere(vec3(0, 0, -1000), 1000, new Scatter(Color(0.5, 0.5, 0.5).sqrt())));
}

int main() {
	Scene scene;
	initScene(scene);

	Image img(1024, 576);

	PerspectiveCamera cam(12, 20*pi/180, 0.1, vec3(12, 3, 2), img.getWidth(), img.getHeight());
	cam.lookAt(vec3(0, 0, 1));

	RayTracing ren2(cam, scene, img, 3);
	ren2.luminaries.push_back(PointLight(vec3(0, 0, 5), Color(0.5, 0.5, 0.5)));
	ren2.render();
	img.colorCorrection();
	saveAsBmp(img, "standard_ray.bmp");

	PathTracing ren(cam, scene, img, 400);
	ren.luminaries.push_back(PointLight(vec3(0, 0, 5), Color(0.5, 0.5, 0.5)));
	ren.render();
	img.colorCorrection();
	saveAsBmp(img, "standard_path.bmp");
}