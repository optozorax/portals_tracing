#include <iostream>
#include <random>


#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/material/reflect.h>
#include <pt/material/refract.h>
#include <pt/material/scatter.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/pt.h>
#include <pt/pt2easybmp.h>
#include <pt/renderer.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>

using namespace pt;

//-----------------------------------------------------------------------------
float myrandom() {
	static std::mt19937 generator(1);
	static std::uniform_real_distribution<float> distribution(0, 1);
	return distribution(generator);
}

//-----------------------------------------------------------------------------
void makeScene1(Scene& scene) {
	double k_coef = 0.8;
	double s_coef = 0.2;
	auto& array = scene.array;
	array.push_back(makeSky(Color(1, 1, 1), Color(0.5, 0.7, 1).sqrt()));

	for(int i = -11; i < 11; ++i) {
		for(int j = -11; j < 11; ++j) {
			vec3 c(i + 0.9 * myrandom(), j + 0.9 * myrandom(), 0.2);
			double p = myrandom();
			if(p < 0.8) {
				array.push_back(makeSphere(c, 0.2, makeScatter(Color(myrandom(), myrandom(), myrandom()).sqrt(), k_coef, s_coef)));
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
	array.push_back(makeSphere(vec3(-4, 0, 1), 1, makeScatter(Color(0.4, 0.2, 0.1).sqrt(), k_coef, s_coef)));
	array.push_back(makeSphere(vec3(4, 0, 1), 1, makeReflect(Color(0.7, 0.6, 0.5).sqrt(), 0)));
	array.push_back(makeSphere(vec3(0, 0, -1000), 1000, makeScatter(Color(0.5, 0.5, 0.5).sqrt(), k_coef, s_coef)));
}

//-----------------------------------------------------------------------------
void makeCam1(PerspectiveCamera& cam1, Orthogonal& cam2, Camera360& cam3, const Image& img) {
	vec3 lookAt(0, 0, 1);
	vec3 pos(12, 3, 2);
	vec3 sppos = spob::cartesian2spheric(pos - lookAt);

	// Инициализируем камеру с песпективной проекцией
	double beta = pi/6;
	double alpha = pi;
	cam1 = PerspectiveCamera(12, 20*pi/180.0, 0.1, lookAt + spheric2cartesian(vec3(sppos.x + 0 * pi/180, sppos.y, sppos.z)), img.getWidth(), img.getHeight());
	cam1.lookAt(lookAt);

	// Инициализируем камеру с ортогональной проекцией
	cam2 = Orthogonal(pos, 0.03 * img.getHeight() / 500.0, img.getWidth(), img.getHeight());
	cam2.lookTowards(lookAt);

	// Инициализируем камеру с проекцией на сферу
	cam3 = Camera360(0.5*(pos - lookAt) + lookAt, img.getHeight());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	Image img(500, 250);
	Image img2(500, 250);

	Scene scene;
	makeScene1(scene);
	PerspectiveCamera cam1(1, pi / 2.0, 0, vec3(0), img.getWidth(), img.getHeight());
	Orthogonal cam2(vec3(0), 0.006, img.getWidth(), img.getHeight());
	Camera360 cam3(vec3(0), img.getHeight());
	makeCam1(cam1, cam2, cam3, img);
	
	{	
		RayTracing ren(1, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren.setAmbientLight(Color(0.5, 0.5, 0.5, 1));

		ren.assign(&cam1, &scene, &img, &img2);
		ren.render();
		img.colorCorrection();
		toGrayScaleDoubleImg(img2, 500);
		saveAsPng(img, "standard_scene_ray_perspective.png");
		saveAsPng(img2, "standard_scene_ray_perspective_depth.png");

		ren.assign(&cam2, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "standard_scene_ray_orthogonal.png");

		ren.assign(&cam3, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "standard_scene_ray_360.png");
	}

	{
		PathTracing ren(400, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren.luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren.assign(&cam1, &scene, &img);
		ren.render();
		img.colorCorrection();
		saveAsPng(img, "standard_scene_path_perspective.png");
	}

	system("pause");
}