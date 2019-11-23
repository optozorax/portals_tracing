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

class percent_time_analyzer
{
public:
	percent_time_analyzer() : time(0) {}

	void start() {
		time = getCurrentTime();
	}

	void print_header(void) const {
		cout << setfill(' ') << setiosflags(ios_base::right);
		cout << "Percent |" << setw(23) << "Time passed |" << setw(23) << "Approximate time |" << setw(24) << "Time Left |" << endl;
		cout << setfill('-') << setw(79) << '|' << endl;
		cout << setfill(' ');
	}

	void print_percent(double percent) const {
		using namespace std;
		stringstream sout;

		cout << '\r';

		sout.str(std::string());
		sout << setprecision(2) << percent * 100 << "% |";
		cout << setw(9) << sout.str();

		sout.str(std::string());
		sout << getTimeString(getTimePassed(time)) << " |";
		cout << setw(23) << sout.str();

		sout.str(std::string());
		sout << getTimeString(getApproxTime(time, percent)) << " |";
		cout << setw(23) << sout.str();

		sout.str(std::string());
		sout << getTimeString(getLeftTime(time, percent)) << " |";
		cout << setw(24) << sout.str();

		cout << endl;
	}

	void end() {
		using namespace std;
		cout << '\r' << setw(9) << "100% |";

		stringstream sout;
		sout.clear();
		sout << getTimeString(getTimePassed(time)) << " |";
		cout << setw(23) << sout.str();

		cout << setw(23) << "0s |" << setw(24) << "0s |" << endl;
		cout << endl;
	}
private:
	double time;

	typedef std::chrono::high_resolution_clock hrc;

	//-------------------------------------------------------------------------
	static float getCurrentTime(void) {
		static hrc::time_point t = hrc::now();
		return std::chrono::duration<double>(hrc::now() - t).count();
	}

	//-------------------------------------------------------------------------
	static float getTimePassed(float pastTime) {
		return getCurrentTime() - pastTime;
	}

	//-------------------------------------------------------------------------
	static float getApproxTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getTimePassed(pastTime) / percent;
	}

	//-------------------------------------------------------------------------
	static float getLeftTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getApproxTime(pastTime, percent) - getTimePassed(pastTime);
	}

	//-------------------------------------------------------------------------
	static std::string getTimeString(float time) {
		char s[25] = {};
		if (true) {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time / 86400), int(time / 3600) % 24, int(time / 60) % 60, int(time) % 60);
			else
				if (time > 3600)
					sprintf(s, "    %2dh %2dm %2ds", int(time / 3600) % 24, int(time / 60) % 60, int(time) % 60);
				else
					if (time > 60)
						sprintf(s, "        %2dm %2ds", int(time / 60) % 60, int(time) % 60);
					else
						sprintf(s, "            %2ds", int(time) % 60);
		}
		else {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time / 86400), int(time / 3600) % 24, int(time / 60) % 60, int(time) % 60);
			else
				if (time > 3600)
					sprintf(s, "%2dh %2dm %2ds", int(time / 3600) % 24, int(time / 60) % 60, int(time) % 60);
				else
					if (time > 60)
						sprintf(s, "%2dm %2ds", int(time / 60) % 60, int(time) % 60);
					else
						sprintf(s, "%2ds", int(time) % 60);
		}
		return std::string(s);
	}
};

double calcTotalTime(const scene::CamPositions& cp) {
	double sum = 0;
	for (auto& i : cp) 
		sum += i.time;
	return sum;
}

std::pair<int, double> getTimeIndex(const scene::CamPositions& cp, double time) {
	int index = 0;
	for (auto& i : cp) {
		if (time < i.time) 
			return {index, time/i.time};
		time -= i.time;
		index++;
	}
	throw std::exception();
}

spob::vec3 interpolate(const spob::vec3& a, const spob::vec3& b, double percent) {
	return a + (b-a) * percent;
}

typedef std::pair<spob::vec3, spob::vec3> Luminary;
typedef std::vector<Luminary> Luminaries;

scene::json unparse(const Luminary& luminary) {
	scene::json result;
	result["pos"] = scene::unparse(luminary.first);
	result["color"] = scene::unparse(luminary.second);
	return result;
}

scene::json unparse(const Luminaries& luminaries) {
	scene::json result;
	for (auto& i : luminaries) {
		result.push_back(unparse(i));
	}
	return result;
}

Luminary parseLuminary(const scene::json& luminary) {
	Luminary result;
	result.first = scene::parseVec3(luminary["pos"]);
	result.second = scene::parseVec3(luminary["color"]);
	return result;
}

Luminaries parseLuminaries(const scene::json& obj) {
	Luminaries result;
	for (const auto& i : obj)
		result.push_back(parseLuminary(i));
	return result;
}

int main(int argc, char** argv) {
	std::string settingsFile = "settings.json";
	std::string filename = "scene.json";
	if (argc > 1) {
		filename = std::string(argv[1]);
		//std::cout << "Read file `" << filename << "`" << std::endl;
	} else {
		//std::cout << "Please specify file to be rendered in command line arguments." << std::endl;
		//std::cout << "Filename interpreted as `" << filename << "`." << std::endl;
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

	int width, height;
	bool isUsePathTracing;
	bool isDrawDepth;
	int rayTracingSamples;
	int pathTracingSamples;
	int threads;
	bool isLog, isAnimationLog;
	bool teleportLightFromPortals;

	std::optional<int> startFrame = std::nullopt, endFrame = std::nullopt;

	int frameOffset = 0, frameSum = 1;

	bool isAnimationFromCameraData;
	int fps;
	std::string cameraDataFile;

	Luminaries luminaries;

	scene::json settings;
	try {
		std::ifstream fin(settingsFile);
		fin >> settings;
		fin.close();
	} catch (...) {
		std::cout << "Settings file is clear or didn't exists." << std::endl;
		std::cout << "Created standard `settings.json`." << std::endl;
		luminaries.emplace_back(spob::vec3(0, 0, 1), spob::vec3(1.5, 1.5, 1.5));
		luminaries.emplace_back(spob::vec3(0, 1, 2.9), spob::vec3(0.5, 0.5, 0.5));

		settings = scene::json();
		settings["width"] = 1000;
		settings["height"] = int(settings["width"]) / 2; 
		settings["isUsePathTracing"] = false;
		settings["isDrawDepth"] = false;
		settings["rayTracingSamples"] = 2;
		settings["pathTracingSamples"] = 200;
		settings["threads"] = 4;
		settings["isLog"] = true;
		settings["isAnimationLog"] = false;
		settings["teleportLightFromPortals"] = false;
		settings["isAnimationFromCameraData"] = false;
		settings["fps"] = 30;
		settings["cameraDataFile"] = "cam_positions.json";
		settings["luminaries"] = unparse(luminaries);

		scene::json frameCycle;
		frameCycle["offset"] = frameOffset;
		frameCycle["sum"] = frameSum;
		settings["frame_cycle"] = frameCycle;

		settings["start_frame"] = scene::unparse(startFrame);
		settings["end_frame"] = scene::unparse(endFrame);
		
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
	isAnimationLog = settings["isAnimationLog"];
	teleportLightFromPortals = settings["teleportLightFromPortals"];
	isAnimationFromCameraData = settings["isAnimationFromCameraData"];
	fps = settings["fps"];
	cameraDataFile = settings["cameraDataFile"];
	luminaries = parseLuminaries(settings["luminaries"]);
	frameOffset = settings["frame_cycle"]["offset"];
	frameSum = settings["frame_cycle"]["sum"];
	startFrame = scene::parseOptional<int>(settings["start_frame"], scene::parseInt);
	endFrame = scene::parseOptional<int>(settings["end_frame"], scene::parseInt);

	//std::cout << std::endl << std::endl;

	using namespace pt;

	auto draw_frame = [isLog, width, height, isUsePathTracing, teleportLightFromPortals, pathTracingSamples, threads, rayTracingSamples, filename, isDrawDepth, luminaries] (const scene::Frame& frame, const vec3& pos, const vec3& lookAt, int current, int total) {
		if (isLog)
			std::cout << "Rendering " << current << " frame of " << total << " frames total" << std::endl;
		Image img(width, height);
		Image dImg(width, height);
		Scene scene = loadFrame(frame);
		scene.add(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));
	
		PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(lookAt);
		StandardRenderer* ren;
		if (isUsePathTracing)
			ren = new PathTracing(pathTracingSamples, threads, isLog, 30);
		else
			ren = new RayTracing(rayTracingSamples, threads, isLog, 1000);
		ren->setAmbientLight(Color(1, 1, 1, 1));
		for (auto& i : luminaries)
			ren->luminaries.push_back(pt::PointLight(i.first, pt::Color(i.second.x, i.second.y, i.second.z)));

		if (teleportLightFromPortals)
			for (auto& portal : frame.portals)
				ren->addPortal(makePortals(portal.crd1, portal.crd2, portal.polygon, nullptr, nullptr));

		ren->assign(&cam, &scene, &img, &dImg);
		ren->render();

		img.colorCorrection();
		saveAsPng(img, filename + "_" + std::to_string(current) + ".png");
		
		if (isDrawDepth) {
			toGrayScaleDoubleImg(dImg);
			saveAsPng(dImg, filename + "_" + std::to_string(current) + "_depth.png");
			saveAsDoubleImg(&img, filename + "_" + std::to_string(current) + ".double");
		}

		delete ren;
	};

	percent_time_analyzer analyzer;
	analyzer.start();
	if (!isLog && isAnimationLog)
		analyzer.print_header();

	if (!isAnimationFromCameraData) {
		if (!startFrame) startFrame = 0;
		if (!endFrame) endFrame = scenejs.frames.size();

		spob::vec3 lookAt = scenejs.cam_rotate_around;
		spob::vec3 pos = spheric2cartesian(scenejs.cam_spheric_pos) + lookAt;
		for (int i = startFrame.value() + frameOffset; i < endFrame.value(); i += frameSum) {
			if (scenejs.frames[i].center) {
				pos = spheric2cartesian(scenejs.frames[i].center.value()) + lookAt;
			}
			draw_frame(scenejs.frames[i], pos, lookAt, i, scenejs.frames.size());

			if (isLog && isAnimationLog) {
				std::cout << std::endl << "Total percent:" << std::endl;
				analyzer.print_header();
			}
			analyzer.print_percent(double(i) / scenejs.frames.size());
			if (isLog)
				std::cout << std::endl << std::endl;
		}
	} else {
		if (isLog)
			std::cout << "Drawing from camera movements data." << std::endl;
		scene::json js;
		try {
			std::ifstream fin(cameraDataFile);
			fin >> js;
			fin.close();
		} catch (...) {
			std::cout << "File `" << cameraDataFile << "` didn't exists or it not contains the correct json." << std::endl;
			std::cout << "Terminate program." << std::endl;
			system("pause");
			return 1;
		}

		auto camPositions = scene::parseCamPositions(js);
		double totalTime = calcTotalTime(camPositions);
		int frames = int(totalTime * fps);
		camPositions.push_back(camPositions.back());

		if (!startFrame) startFrame = 0;
		if (!endFrame) endFrame = frames;

		for (int i = startFrame.value() + frameOffset; i < endFrame.value(); i += frameSum) {
			double time = double(i)/fps;
			auto res = getTimeIndex(camPositions, time);
			int index = res.first;
			double percent = res.second;

			spob::vec3 lookAt1 = interpolate(camPositions[index].cam_rotate_around, camPositions[index+1].cam_rotate_around, percent);
			spob::vec3 pos1 = spheric2cartesian(interpolate(camPositions[index].cam_spheric_pos, camPositions[index+1].cam_spheric_pos, percent)) + lookAt1;

			draw_frame(scenejs.frames[camPositions[index].frame], pos1, lookAt1, i, frames);

			if (isLog && isAnimationLog) {
				std::cout << std::endl << "Total percent:" << std::endl;
				analyzer.print_header();
			}
			analyzer.print_percent(double(i)/frames);
			if (isLog)
				std::cout << std::endl << std::endl;
		}
	}
}