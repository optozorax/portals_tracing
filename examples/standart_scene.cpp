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
	array.push_back(makeSky(Color(1, 1, 1), Color(0.5, 0.7, 1).sqrt()));

	for(int i = -11; i < 11; ++i) {
		for(int j = -11; j < 11; ++j) {
			vec3 c(i + 0.9 * myrandom(), j + 0.9 * myrandom(), 0.2);
			double p = myrandom();
			if(p < 0.8) {
				array.push_back(makeSphere(c, 0.2, makeScatter(Color(myrandom(), myrandom(), myrandom()).sqrt())));
			} else if(p < 0.95) {
				double r = 0.5 * (1 + myrandom());
				double g = 0.5 * (1 + myrandom());
				double b = 0.5 * (1 + myrandom());
				array.push_back(makeSphere(c, 0.2, makeReflect(Color(r, g, b).sqrt(), 0.5 * myrandom())));
			} else {
				array.push_back(makeSphere(c, 0.2, makeRefract(1.5, 0)));
			}
		}
	}
	array.push_back(makeSphere(vec3(0, 0, 1), 1, makeRefract(1.5, 0)));
	array.push_back(makeSphere(vec3(-4, 0, 1), 1, makeScatter(Color(0.4, 0.2, 0.1).sqrt())));
	array.push_back(makeSphere(vec3(4, 0, 1), 1, makeReflect(Color(0.7, 0.6, 0.5).sqrt(), 0)));
	array.push_back(makeSphere(vec3(0, 0, -1000), 1000, makeScatter(Color(0.5, 0.5, 0.5).sqrt())));
}

int main() {
	Scene scene;
	initScene(scene);

	Image img(1920, 1080);

	PerspectiveCamera cam(12, 20*pi/180, 0.1, vec3(12, 3, 2), img.getWidth(), img.getHeight());
	cam.lookAt(vec3(0, 0, 1));

	{
		RayTracing ren(3, 16, true);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 5), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsBmp(img, "standard_ray.bmp");
	}

	{
		PathTracing ren(400, 16, true);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 5), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsBmp(img, "standard_path.bmp");
	}
}