#include <sstream>
#include <algorithm>
#include <random>
#include <iostream>
#include <stack>
#include <chrono>
#include <string>
#include <iomanip>
#include <mutex>
#include <thread>

#include <pt/renderer.h>

namespace pt
{

namespace {
    typedef std::chrono::high_resolution_clock hrc;

	//-------------------------------------------------------------------------
	float getCurrentTime(void) {
		static hrc::time_point t = hrc::now();
		return std::chrono::duration<double>(hrc::now() - t).count();
	}

	//-------------------------------------------------------------------------
	float getTimePassed(float pastTime) {
		return getCurrentTime() - pastTime;
	}

	//-------------------------------------------------------------------------
	float getApproxTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getTimePassed(pastTime)/percent;
	}

	//-------------------------------------------------------------------------
	float getLeftTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getApproxTime(pastTime, percent) - getTimePassed(pastTime);
	}

	//-------------------------------------------------------------------------
	std::string getTimeString(float time) {
		char s[25] = {};
		if (true) {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time/86400), int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 3600)
				sprintf(s, "    %2dh %2dm %2ds", int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 60)
				sprintf(s, "        %2dm %2ds", int(time/60)%60, int(time)%60);
			else
				sprintf(s, "            %2ds", int(time)%60);
		} else {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time/86400), int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 3600)
				sprintf(s, "%2dh %2dm %2ds", int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 60)
				sprintf(s, "%2dm %2ds", int(time/60)%60, int(time)%60);
			else
				sprintf(s, "%2ds", int(time)%60);
		}
		return std::string(s);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
StandardRenderer::StandardRenderer(
	int threads,
	int maxDepth,
	double tMax,
	bool isDiffuse, 
	bool isBreakOnMaterial,
	bool isWriteText) : 
	threads(threads),
	maxDepth(maxDepth),
	tMax(tMax),
	isDiffuse(isDiffuse),
	isBreakOnMaterial(isBreakOnMaterial),
	isWriteText(isWriteText),
	ambient(0, 0, 0, 1){
}

//-----------------------------------------------------------------------------
StandardRenderer::~StandardRenderer() {
}

//-----------------------------------------------------------------------------
void StandardRenderer::addPortal(Portals_ptr portal) {
	portals.push_back(portal);
	Portals_ptr inv = std::make_shared<Portals>(invert(*portal));
	invertedPortals.push_back(inv);
}

//-----------------------------------------------------------------------------
void StandardRenderer::clearPortals(void) {
	portals.clear();
	invertedPortals.clear();
}

//-----------------------------------------------------------------------------
void StandardRenderer::render(void) {
	std::vector<int> pixels(img->getWidth() * img->getHeight(), 0);
	for (int i = 0; i < pixels.size(); ++i)
		pixels[i] = i;

	static std::random_device rd;
	static std::mt19937 g(rd());

	std::shuffle(pixels.begin(), pixels.end(), g);

	std::mutex write_mutex;
	int renderedPixelsCount = 0;

	onStartRender();
	std::vector<std::thread> threads_mas;
	for (int i = 0; i < threads; ++i) {
		threads_mas.push_back(std::thread([&] (int i) {
			for (int j = i; j < pixels.size(); j += threads) {
				write_mutex.lock();
				renderedPixelsCount++;
				if (renderedPixelsCount % img->getHeight() == 0)
					onEveryLine(double(renderedPixelsCount)/pixels.size());
				write_mutex.unlock();

				int x = pixels[j] % img->getWidth();
				int y = pixels[j] / img->getWidth();
				auto pix = computePixel(x, y);
				(*img)(x, y) = pix.color;
				if (isDrawDepth) 
					(*dImg)(x, y) = Color(pix.depth, 0, 0);
			}
		}, i));
	}
	for (auto& i : threads_mas)
		i.join();
	onEndRendering();
}

//-----------------------------------------------------------------------------
void StandardRenderer::onStartRender(void) {
	using namespace std;
	if (isWriteText) {
		time = getCurrentTime();
		cout << setfill(' ') << setiosflags(ios_base::right);
		cout << "Percent |" << setw(23) << "Time passed |" << setw(23) << "Approximate time |" << setw(24) << "Time Left |" << endl;
		cout << setfill('-') << setw(79) << '|' << endl;
		cout << setfill(' ');
	}
}

//-----------------------------------------------------------------------------
void StandardRenderer::onEveryLine(double percent) const {
	using namespace std;
	if (isWriteText) {
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
	}
}

//-----------------------------------------------------------------------------
void StandardRenderer::onEndRendering(void) const {
	using namespace std;
	if (isWriteText) {
		cout << '\r' << setw(9) << "100% |";

		stringstream sout;
		sout.clear();
		sout << getTimeString(getTimePassed(time)) << " |";
		cout << setw(23) << sout.str();

		cout << setw(23) << "0s |" << setw(24) << "0s |" << endl;
		cout << endl;
	}
}

//-----------------------------------------------------------------------------
Renderer1::Frag StandardRenderer::computePixel(int x, int y) const {
	return computeColor(camera->getRay(x, y, isDiffuse));
}

//-----------------------------------------------------------------------------
Renderer1::Frag StandardRenderer::computeColor(Ray ray) const {
	Intersection inter;
	Color clrAbsorbtion;
	std::stack<Color> colorStack;
	std::stack<Color> pointLightColorStack;
	std::stack<bool> pointLightColorStackBool;
	Ray scattered;
	double diffusion;
	ScatterType returned;

	double depth = tMax;
	bool isDepthInitialized = false;
	
	for (int i = 0; i < maxDepth; ++i) {
		if (scene->intersect(ray, inter, 0, tMax)) {
			if (!isDepthInitialized) {
				depth = inter.t;
				isDepthInitialized = true;
			}

			returned = scene->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);

			// Запоминаем прозрачность текущего цвета
			double opaque = clrAbsorbtion.a;
			clrAbsorbtion.a = 1;

			// Изменить направление в соответствии с рассеиванием
			if (isDiffuse)
				scattered.dir += randomSphere() * diffusion;
			scattered.dir.normalize();

			// Сместить положение луча в некотором направлении
			scattered.pos += scattered.dir * 0.00001;

			// Считаем цвет освещения, но его надо считать только когда у нас обычный материал
			if (returned == SCATTER_RAYTRACING_END && luminaries.size() != 0) {
				Color lightColor = Color(0, 0, 0, 1);
				std::vector<std::pair<Portals_ptr, vec3> > teleportation;
				lightColor += computeLight(scattered.pos, inter.normal, teleportation, 3);
				pointLightColorStack.push(lightColor * clrAbsorbtion);
				pointLightColorStackBool.push(true);
			} else {
				pointLightColorStack.push(Color(0, 0, 0, 0));
				pointLightColorStackBool.push(false);
			}

			// Посчитать результирующй цвет после данного отражения
			colorStack.push(clrAbsorbtion);

			// Если полигон полупрозрачный, то его цвет будет комбинацией двух лучей, сложенных с учетом прозрачности
			if (opaque != 1.0) {
				// Отложим поддержку прозрачности до лучших времен
				/*// Изменяем и сохраняем глубину рендерера, чтобы он не зациклился
				int temp = maxDepth;
				maxDepth -= i + 1; 

				// Получаем цвет луча, который пойдет сквозь материал
				Color throughColor;
				if (returned != SCATTER_END) {
					Ray through;
					through.pos = scattered.pos;
					through.dir = ray.dir;
					through.pos += through.dir * 0.00001;
					throughColor = computeColor(through);
				}

				// Получаем цвет луча, который пошел бы обычным путем
				Color nextColor;
				if (!(returned == SCATTER_RAYTRACING_END && isBreakOnMaterial))
					nextColor = computeColor(scattered);

				// Совмещаем два цвета с учетом прозрачности
				resultColor = resultColor * clrAbsorbtion;
				resultColor.a = opaque;
				resultColor = overlay(resultColor, throughColor);

				maxDepth = temp;
				break;*/
			}

			ray = scattered;
			if (returned == SCATTER_END || (returned == SCATTER_RAYTRACING_END && isBreakOnMaterial))
				break;
		} else {
			if (i == 0)
				return Frag(Color(0, 0, 0, 0), depth);
			else
				break;
		}
	}

	Color resultColor(1, 1, 1, 1);
	if (returned == SCATTER_END) {
		resultColor = colorStack.top();

		colorStack.pop();
		pointLightColorStack.pop();
		pointLightColorStackBool.pop();
	} else {
		resultColor = ambient;
	}
	while (!colorStack.empty()) {
		if (pointLightColorStackBool.top()) {
			resultColor = colorStack.top() * resultColor;
			resultColor += pointLightColorStack.top();
			resultColor /= 2.0;
		} else {
			resultColor = colorStack.top() * resultColor;
		}

		colorStack.pop();
		pointLightColorStack.pop();
		pointLightColorStackBool.pop();
	}

	return Frag(resultColor, depth);
}

//-----------------------------------------------------------------------------
Color StandardRenderer::computeLight(
	vec3 pos, vec3 normal,
	std::vector<std::pair<Portals_ptr, vec3> >& teleportation,
	int depth) const {
	// В этой функции предполагается, что все источники света должны быть телепортированы через порталы, указанные в teleportation, и для всех них как раз проверяется, чтобы через все эти порталы свет мог попасть к текущему месту, которое проверяется на освещенность

	// Тут тоже отложим поддержку прозрачности до лучших времен
	Color result(0, 0, 0, 0);
	Intersection inter;
	double t;
	double cosine;
	bool isIntersect;
	Ray ray;

	for (auto i : luminaries) {
		// Эта переменная отвечает за то, чтобы луч света проходил через все текущие порталы, если она на миг становится false, то перебор данного источника света сразу прекращается
		bool isPass = true;
		for (int j = teleportation.size() - 1; j >= 0; --j) {
			// Получаем луч, который идет от текущего телепортированного положения до текущего источника света
			vec3& pos = teleportation[j].second;
			Portals& portal = *teleportation[j].first;
			ray = {pos, i.pos - pos};
			ray.dir.normalize();

			// Проверяем, чтобы этот луч входил в портал
			isPass &= dot(ray.dir, portal.p2.k) > 0;
			if (!isPass) goto next;
			isPass &= portal.pg2.intersect(ray, inter, 0, tMax);
			if (!isPass) goto next;

			// Проверяем, чтобы на пути от портала до текущего положения источника света не было препятствий
			ray.pos += ray.dir * (inter.t + 0.00001);
			t = distance(i.pos, ray.pos);
			isIntersect = scene->intersect(ray, inter, 0, t + 1);
			isPass &= !isIntersect || (isIntersect && inter.t > t);
			if (!isPass) goto next;

			// Сдвигаем источник света по лучу ближе к текущему месту, на освещенность данного конкретного места не повлияет, а после сдвига телепортируем, чтобы рассчитывать это для других порталов
			i.pos -= ray.dir * (t + 0.00003);
			i.pos = portal.p1.from(portal.p2.to(i.pos));
		}

		// Получаем луч от текущего абсолютного места до текущего источника света
		ray = {pos, i.pos - pos};
		ray.dir.normalize();
		normal.normalize();
		ray.pos += ray.dir * 0.00001;

		// Проверяем, чтобы на пути не было препятствий
		t = distance(i.pos, pos);
		isIntersect = scene->intersect(ray, inter, 0, t + 1);
		isPass &= !isIntersect || (isIntersect && inter.t > t);
		if (!isPass) goto next;

		// Если всех этих препятствий нет, то прибавляем освещение от этого источника света
		cosine = dot(ray.dir, normal);
		result += i.clr * cosine;

		next:;
	}

	// Далее, если позволяет глубина, перебираем все порталы дальше
	if (depth > 0) {
		for (int i = 0; i < portals.size(); ++i) {
			auto recursion = [&] (Portals_ptr portal) {
				vec3 newPos;
				if (teleportation.size() != 0)
					newPos = portal->p2.from(portal->p1.to(teleportation.back().second));
				else
					newPos = portal->p2.from(portal->p1.to(pos));
				teleportation.push_back({portal, newPos});
				result += computeLight(pos, normal, teleportation, depth-1);
				teleportation.pop_back();
			};

			// Перебираем прямой и обратный порядок следования порталов
			recursion(portals[i]);
			recursion(invertedPortals[i]);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
void StandardRenderer::setAmbientLight(Color clr) {
	ambient = clr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RayTracing::RayTracing(int aliasing,
					   int threads,
					   bool isWriteText,
					   int maxDepth,
					   double tMax) : StandardRenderer(threads, maxDepth, tMax, false, true, isWriteText), antialiasing(aliasing) {
}

//-----------------------------------------------------------------------------
Renderer1::Frag RayTracing::computePixel(int x, int y) const {
	Color clr(0, 0, 0, 0);
	double depth = tMax;
	for (int ki = 0; ki < antialiasing; ++ki) {
		for (int kj = 0; kj < antialiasing; ++kj) {
			double x1 = x + double(ki)/antialiasing;
			double y1 = y + double(kj)/antialiasing;
			Ray ray = camera->getRay(x1, y1, isDiffuse);
			auto pix = computeColor(ray);
			clr += pix.color;
			depth = std::min(depth, pix.depth);
		}
	}
	clr /= antialiasing * antialiasing;
	return Frag(clr, depth);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
PathTracing::PathTracing(int samples,
						 int threads,
						 bool isWriteText,
						 int maxDepth,
						 double tMax) : StandardRenderer(threads, maxDepth, tMax, true, false, isWriteText), samples(samples) {
}

//-----------------------------------------------------------------------------
Renderer1::Frag PathTracing::computePixel(int x, int y) const {
	// Использована квазислучайная последовательность https://habr.com/ru/post/440892/
	const double g = 1.32471795724474602596090885447809;
	const double ax = std::fmod(1.0/g, 1.0);
	const double ay = std::fmod(1.0/g/g, 1.0);
	const double seed = 0.5;

	Color clr(0, 0, 0, 0);
	double depth = tMax;
	for (int i = 0; i < samples; ++i) {
		double x1 = x + std::fmod(seed + ax*i, 1.0);
		double y1 = y + std::fmod(seed + ay*i, 1.0);
		Ray ray = camera->getRay(x1, y1, isDiffuse);
		auto pix = computeColor(ray);
		clr += pix.color;
		depth = std::min(depth, pix.depth);
	}
	clr /= samples;
	return Frag(clr, depth);
}

};