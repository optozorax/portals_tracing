#include <fstream>
#include <iostream>
#include <iomanip>

#include <pt/pt.h>
#include <pt/object/scene.h>
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

int main(int argc, char** argv) {
	std::string settingsFile = "settings.json";
	std::string filename = "scene.json";
	if (argc > 1) {
		filename = std::string(argv[1]);
		std::cout << "Read file `" << filename << "`" << std::endl;
	} else {
		std::cout << "Please specify file to be rendered in command line arguments." << std::endl;
		std::cout << "Filename interpreted as `" << filename << "`." << std::endl;
	}

	scene::json js;
	try {
		std::ifstream fin(filename);
		fin >> js;
		fin.close();
	} catch (...) {
		std::cout << "File `scene.json` didn't exists or it not contains the scene." << std::endl;
		std::cout << "Terminate program." << std::endl;
		system("pause");
		return 1;
	}

	auto scenejs = scene::parseScene(js);
	spob::vec3 lookAt = scenejs.cam_rotate_around;
	spob::vec3 pos = spheric2cartesian(scenejs.cam_spheric_pos) + lookAt;

	int width, height;
	bool isUsePathTracing;
	bool isDrawDepth;
	int rayTracingSamples;
	int pathTracingSamples;
	int threads;
	bool isLog;

	scene::json settings;
	try {
		std::ifstream fin(settingsFile);
		fin >> settings;
		fin.close();
	} catch (...) {
		std::cout << "Settings file is clear or didn't exists." << std::endl;
		std::cout << "Created standard `settings.json`." << std::endl;
		settings = scene::json();
		settings["width"] = 1000;
		settings["height"] = int(settings["width"]) / 2; 
		settings["isUsePathTracing"] = false;
		settings["isDrawDepth"] = false;
		settings["rayTracingSamples"] = 2;
		settings["pathTracingSamples"] = 200;
		settings["threads"] = 4;
		settings["isLog"] = true;
		std::ofstream fout(settingsFile);
		fout << std::setw(4) << settings;
		fout.close();

		system("pause");
	}

	width = settings["width"];
	height = settings["height"];
	isUsePathTracing = settings["isUsePathTracing"];
	isDrawDepth = settings["isDrawDepth"];
	rayTracingSamples = settings["rayTracingSamples"];
	pathTracingSamples = settings["pathTracingSamples"];
	threads = settings["threads"];
	isLog = settings["isLog"];

	std::cout << std::endl << std::endl;

	using namespace pt;

	for (int i = 0; i < scenejs.frames.size(); i++) {
		if (isLog)
			std::cout << "Rendering " << i << " frame of " << scenejs.frames.size() << " frames total" << std::endl;;
		Image img(width, height);
		Image dImg(width, height);
		Scene scene = loadFrame(scenejs.frames[i]);
		scene.add(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
	
		PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(lookAt);
		StandardRenderer* ren;
		if (isUsePathTracing)
			ren = new PathTracing(pathTracingSamples, threads, isLog, 30);
		else
			ren = new RayTracing(rayTracingSamples, threads, isLog, 100);
		ren->setAmbientLight(Color(1, 1, 1, 1));
		ren->luminaries.push_back(PointLight(vec3(0, 0, 1), Color(1.5, 1.5, 1.5)));
		ren->luminaries.push_back(PointLight(vec3(0, 1, 2.9), Color(0.5, 0.5, 0.5)));
		ren->assign(&cam, &scene, &img, &dImg);
		ren->render();

		img.colorCorrection();
		saveAsPng(img, filename + "_" + std::to_string(i) + ".png");
		
		if (isDrawDepth) {
			toGrayScaleDoubleImg(dImg);
			saveAsPng(dImg, filename + "_" + std::to_string(i) + "_depth.png");
			saveAsDoubleImg(&img, filename + "_" + std::to_string(i) + ".double");
		}

		delete ren;
	}
}