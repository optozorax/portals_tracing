#include <fstream>
#include <iostream>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/shape/cone.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/material/light.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

#include <prtl_vis/scene_reader.h>

int main(int argc, char** argv) {
	std::string filename = "test12.json";
	if (argc > 1)
		filename = std::string(argv[1]);

	scene::json js;
	std::ifstream fin(filename);
	fin >> js;
	fin.close();

	auto scenejs = scene::parseScene(js);
	spob::vec3 lookAt = scenejs.cam_rotate_around;
	spob::vec3 pos = spheric2cartesian(scenejs.cam_spheric_pos) + lookAt;

	int width = 500, height = 250;
	bool isUsePathTracing = false;
	int rayTracingSamples = 1;
	int pathTracingSamples = 2000;
	int threads = 1;
	bool isLog = true;

	using namespace pt;

	for (int i = 0; i < scenejs.frames.size(); i++) {
		if (isLog)
			std::cout << "Rendering " << i << " frame of " << scenejs.frames.size() << " frames total" << std::endl;;
		Image img(width, height);
		Scene scene = loadFrame(scenejs.frames[i]);
		scene.add(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
		double size = 5;
		std::vector<vec2> square = {{0, 0}, {0, size}, {size, size}, {size, 0},};
		auto crd = getStandardCrd3();
		crd.move(vec3(0, 0, 3));
		//scene.add(makePolygon(square, crd, makeLight(Color(1.5, 1.5, 1.5))));
	
		PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(lookAt);
		StandardRenderer* ren;
		if (isUsePathTracing)
			ren = new PathTracing(pathTracingSamples, threads, isLog, 4);
		else
			ren = new RayTracing(rayTracingSamples, threads, isLog, 100);
		ren->luminaries.push_back(PointLight(vec3(0, 0, 3), Color(1.5, 1.5, 1.5)));
		ren->luminaries.push_back(PointLight(vec3(0, 1, 3), Color(0.5, 0.5, 0.5)));
		ren->assign(&cam, &scene, &img);
		ren->render();
		img.colorCorrection();
		saveAsPng(img, filename + "_" + std::to_string(i) + ".png");
	}
}