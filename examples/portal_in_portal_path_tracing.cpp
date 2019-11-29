#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/scene2.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/material/light.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

#include <prtl_vis/scene_reader.h>

using namespace std;

int main(int argc, char** argv) {
	std::string filename = "for_path_tracing.json";

	scene::json js;

	std::ifstream fin(filename);
	fin >> js;
	fin.close();

	auto scenejs = scene::parseScene(js);

	int width = 1000, height = 1000;
	int rayTracingSamples = 2;
	bool isUsePathTracing = true;
	int pathTracingSamples = 4000;
	int threads = 3;

	using namespace pt;

	spob::vec3 lookAt = scenejs.frames.back().center.value();
	auto sph = scenejs.cam_spheric_pos;
	sph.z = 2;
	spob::vec3 pos = spheric2cartesian(sph) + lookAt;

	Image img(width, height);
	Scene scene = loadFrame(scenejs.frames.back());

	// Добавление источника света
	auto potolok = scenejs.frames.back().colored_polygons[2];
	scene.array.erase(scene.array.begin() + 2);
	auto crd = potolok.crd;
	crd.pos.z -= 0.01;
	auto poly = potolok.polygon;
	scene.array.push_back(makePolygon(poly, crd, makeLight(Color(1, 1, 1))));
	
	PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
	cam.lookAt(lookAt);
	//Camera360 cam(pos, img.getHeight());
	StandardRenderer* ren;
	if (isUsePathTracing)
		ren = new PathTracing(pathTracingSamples, threads, true, 10);
	else {
		ren = new RayTracing(rayTracingSamples, threads, false, 1000);
		ren->luminaries.push_back(PointLight(vec3(0, 0, 1), Color(1.5, 1.5, 1.5)));
		ren->luminaries.push_back(PointLight(vec3(0, 1, 2.9), Color(0.5, 0.5, 0.5)));
	}
	ren->setAmbientLight(Color(1, 1, 1, 1));

	ren->assign(&cam, &scene, &img);
	ren->render();

	img.colorCorrection();
	saveAsPng(img, "portal_in_portal_path_tracing.png");
}