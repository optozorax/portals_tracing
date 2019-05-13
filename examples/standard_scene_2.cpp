#include <iostream>
#include <random>

#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/material/reflect.h>
#include <pt/material/refract.h>
#include <pt/material/scatter.h>
#include <pt/material/light.h>
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
void rect(Scene& scene, vec3 a, vec3 b, vec3 c, Material_ptr m) {
	std::swap(a.y, a.z);
	std::swap(b.y, b.z);
	std::swap(c.y, c.z);
	//std::vector<vec2> mas = {a, b, c, b - (b-(c+a)*0.5)*2};
	//scene.add(makeTriangle(a, b, c, m));
	//scene.add(makeTriangle(a, b - (b-(c+a)*0.5)*2, c, m));
	crd3 crd = spob::makePlane3(b, a, c);
	double il = crd.i.length();
	double jl = crd.j.length();
	std::vector<vec2> mas = {{0, 0}, {0, jl}, {il, jl}, {il, 0}};
	crd.i.normalize();
	crd.j.normalize();
	crd.k.normalize();
	scene.add(makePolygon(mas, crd, m));
}

//-----------------------------------------------------------------------------
void makeScene1(Scene& scene) {
	double k_coef = 1;
	double s_coef = 0;

	// FLoor
	rect(scene, 
		vec3(-2, 0, -2),
		vec3(-2, 0, 2),
		vec3(20, 0, 2),
		makeScatter(Color(0.7f, 0.7f, 0.7f), k_coef, s_coef)
	);

	// Ceiling
	rect(scene, 
		vec3(-2, 4, -2),
		vec3(-2, 4, 2),
		vec3(20, 4, 2),
		makeScatter(Color(0.9f, 0.9f, 0.9f), k_coef, s_coef)
	);

	// Front wall
	rect(scene, 
		vec3(-2, 0, -2),
		vec3(-2, 0, 2),
		vec3(-2, 4, 2),
		makeScatter(Color(0.7f, 0.7f, 0.7f), k_coef, s_coef)
	);

	// Left wall
	rect(scene, 
		vec3(-2, 0, -2),
		vec3(20, 0, -2),
		vec3(20, 4, -2),
		makeScatter(Color(0.9f, 0.1f, 0.1f), k_coef, s_coef)
	);

	// Right wall
	rect(scene, 
		vec3(-2, 0, 2),
		vec3(20, 0, 2),
		vec3(20, 4, 2),
		makeScatter(Color(0.1f, 0.9f, 0.1f), k_coef, s_coef)
	);

	scene.add(makeSphere(
		vec3(-1, 1, 1), 1, 
		makeReflect(Color(1, 1, 1), 0)
	));

	scene.add(makeSphere(vec3(-1.3, -1.3, 0.7), 0.7, makeScatter(Color(0.1, 0.1, 0.9), k_coef, s_coef)));
	scene.add(makeSphere(vec3(1, 0.2, 0.5), 0.5, makeRefract(1.5, 0)));

	float lampSize = 1;
	rect(scene, 
		vec3(-lampSize, 3.95, -lampSize),
		vec3(-lampSize, 3.95, lampSize),
		vec3(lampSize, 3.95, lampSize),
		makeLight(Color(1, 1, 1))
	);

	scene.add(makeSky(Color(0, 0, 0), Color(0, 0, 0)));
}

//-----------------------------------------------------------------------------
void makeCam1(PerspectiveCamera& cam1, Orthogonal& cam2, Camera360& cam3, const Image& img, const Image& img2) {
	vec3 lookAt(0, 0, 2);
	vec3 pos(15, 0, 2);
	vec3 sppos = spob::cartesian2spheric(pos - lookAt);

	// Инициализируем камеру с песпективной проекцией
	double beta = pi/6;
	double alpha = pi;
	cam1 = PerspectiveCamera(12, 20*pi/180.0, 0.1, lookAt + spheric2cartesian(vec3(sppos.x + 0 * pi/180, sppos.y, sppos.z)), img.getWidth(), img.getHeight());
	cam1.lookAt(lookAt);

	// Инициализируем камеру с ортогональной проекцией
	cam2 = Orthogonal(pos, 0.01 * img.getHeight() / 500, img.getWidth(), img.getHeight());
	cam2.lookTowards(lookAt);

	// Инициализируем камеру с проекцией на сферу
	cam3 = Camera360(0.18*(pos - lookAt) + lookAt, img2.getHeight());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	Image img(500, 500);
	Image img2(4000, 2000);

	Scene scene;
	makeScene1(scene);
	PerspectiveCamera cam1(1, pi / 2.0, 0, vec3(0), img.getWidth(), img.getHeight());
	Orthogonal cam2(vec3(0), 0.006, img.getWidth(), img.getHeight());
	Camera360 cam3(vec3(0), img.getHeight());
	makeCam1(cam1, cam2, cam3, img, img2);
	
	{	
		RayTracing ren(4, 4);
		ren.luminaries.push_back(PointLight(vec3(0, 0, 3.94), Color(1, 1, 1)));
		ren.setAmbientLight(Color(0.5, 0.5, 0.5, 1));

		ren.assign(&cam1, &scene, &img);
		//ren.render();
		img.colorCorrection();
		//saveAsPng(img, "standard_scene_2_ray_perspective.png");

		ren.assign(&cam2, &scene, &img);
		//ren.render();
		img.colorCorrection();
		//saveAsPng(img, "standard_scene_2_ray_orthogonal.png");

		ren.assign(&cam3, &scene, &img2);
		ren.render();
		img2.colorCorrection();
		saveAsPng(img2, "standard_scene_2_ray_360.png");
	}

	{
		PathTracing ren(1000, 3, true, 5);
		ren.luminaries.clear();
		ren.assign(&cam1, &scene, &img);
		//ren.render();
		img.colorCorrection();
		//saveAsPng(img, "standard_scene_2_path_perspective.png");
	}
}