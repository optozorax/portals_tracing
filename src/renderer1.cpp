#include <sstream>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>

#include <pt/renderer1.h>

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
StandardRendererWithPointLight::StandardRendererWithPointLight(const Camera& camera, 
	const Object& scene,
	Image& img, 
	int maxDepth, 
	double tMax,
	bool isDiffuse, 
	bool isBreakOnMaterial) : 
	Renderer1(camera, scene, img),
	maxDepth(maxDepth),
	tMax(tMax),
	isDiffuse(isDiffuse),
	isBreakOnMaterial(isBreakOnMaterial) {
}

//-----------------------------------------------------------------------------
void StandardRendererWithPointLight::render(void) {
	std::vector<int> pixels(img.getWidth() * img.getHeight(), 0);
	for (int i = 0; i < pixels.size(); ++i)
		pixels[i] = i;
	std::random_shuffle(pixels.begin(), pixels.end());

	onStartRender();
	for (int i = 0; i < pixels.size(); ++i) {
		if (i % img.getHeight() == 0)
			onEveryLine(double(i)/pixels.size());
		int x = i % img.getWidth();
		int y = i / img.getWidth();
		img(x, y) = computePixel(x, y);
	}
	onEndRendering();
}

//-----------------------------------------------------------------------------
void StandardRendererWithPointLight::onStartRender(void) {
	using namespace std;
	time = getCurrentTime();
	cout << setfill(' ') << setiosflags(ios_base::right);
	cout << "Percent |" << setw(23) << "Time passed |" << setw(23) << "Approximate time |" << setw(24) << "Time Left |" << endl;
	cout << setfill('-') << setw(79) << '|' << endl;
	cout << setfill(' ');
}

//-----------------------------------------------------------------------------
void StandardRendererWithPointLight::onEveryLine(double percent) {
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
}

//-----------------------------------------------------------------------------
void StandardRendererWithPointLight::onEndRendering(void) {
	using namespace std;
	cout << '\r' << setw(9) << "100% |";

	stringstream sout;
	sout.clear();
	sout << getTimeString(getTimePassed(time)) << " |";
	cout << setw(23) << sout.str();

	cout << setw(23) << "0s |" << setw(24) << "0s |" << endl;
	cout << endl;
}

//-----------------------------------------------------------------------------
Color StandardRendererWithPointLight::computePixel(int x, int y) {
	return computeColor(camera.getRay(x, y, isDiffuse));
}

//-----------------------------------------------------------------------------
Color StandardRendererWithPointLight::computeColor(Ray ray) {
	Color resultColor = Color(1, 1, 1, 1);
	Intersection inter;
	Color clrAbsorbtion;
	Ray scattered;
	double diffusion;
	ScatterType returned;
	
	for (int i = 0; i < maxDepth; ++i) {
		if (scene.intersect(ray, inter, 0, tMax)) {
			returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);

			if (returned == SCATTER_NEXT)
				returned = returned;

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
			if (returned == SCATTER_RAYTRACING_END) {
				Color lightColor = Color(1, 1, 1, 1);
				std::vector<std::pair<Portals, vec3> > teleportation;
				lightColor += computeLight(scattered.pos, inter.normal, teleportation, 3);
				clrAbsorbtion = lightColor * clrAbsorbtion;
			}

			// Посчитать результирующй цвет после данного отражения
			resultColor = clrAbsorbtion * resultColor;

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
				resultColor = Color(0, 0, 0, 0);
			break;
		}
	}

	return resultColor;
}

//-----------------------------------------------------------------------------
Color StandardRendererWithPointLight::computeLight(
	vec3 pos, vec3 normal,
	std::vector<std::pair<Portals, vec3> >& teleportation,
	int depth) {
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
			Portals& portal = teleportation[j].first;
			ray = {pos, i.pos - pos};
			ray.dir.normalize();

			// Проверяем, чтобы этот луч входил в портал
			isPass &= dot(ray.dir, portal.p2.k) > 0;
			if (!isPass) goto next;
			isPass &= portal.pg2.intersect(ray, inter, 0, tMax);
			if (!isPass) goto next;

			// Проверяем, чтобы на пути от портала до текущего положения источника света не было препятствий
			ray.pos += ray.dir * (inter.t + 0.00001);
			t = (i.pos - ray.pos).getLength();
			isIntersect = scene.intersect(ray, inter, 0, t + 1);
			isPass &= !isIntersect || (isIntersect && inter.t > t);
			if (!isPass) goto next;

			// Сдвигаем источник света по лучу ближе к текущему месту, на освещенность данного конкретного места не повлияет, а после сдвига телепортируем, чтобы рассчитывать это для других порталов
			i.pos -= ray.dir * (t + 0.00003);
			i.pos = teleportVector(portal.p2, portal.p1, i.pos);
		}

		// Получаем луч от текущего абсолютного места до текущего источника света
		ray = {pos, i.pos - pos};
		ray.dir.normalize();
		normal.normalize();
		ray.pos += ray.dir * 0.00001;

		// Проверяем, чтобы на пути не было препятствий
		t = (i.pos - pos).getLength();
		isIntersect = scene.intersect(ray, inter, 0, t + 1);
		isPass &= !isIntersect || (isIntersect && inter.t > t);
		if (!isPass) goto next;

		// Если всех этих препятствий нет, то прибавляем освещение от этого источника света
		cosine = dot(ray.dir, normal);
		result += i.clr * cosine;

		next:;
	}

	// Далее, если позволяет глубина, перебираем все порталы дальше
	if (depth > 0) {
		for (auto j : portals) {
			auto recursion = [&] (const Portals& portal) {
				vec3 newPos;
				if (teleportation.size() != 0)
					newPos = teleportVector(portal.p1, portal.p2, teleportation.back().second);
				else
					newPos = teleportVector(portal.p1, portal.p2, pos);
				teleportation.push_back({portal, newPos});
				result += computeLight(pos, normal, teleportation, depth-1);
				teleportation.pop_back();
			};

			// Перебираем прямой и обратный порядок следования порталов
			recursion(*j);
			recursion(invert(*j));
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RayTracing::RayTracing(const Camera& camera, 
					   const Object& scene,
					   Image& img, 
					   int aliasing,
					   int maxDepth,
					   double tMax) : StandardRendererWithPointLight(camera, scene, img, maxDepth, tMax, 0, true), antialiasing(aliasing) {
}

//-----------------------------------------------------------------------------
Color RayTracing::computePixel(int x, int y) {
	Color clr(0, 0, 0, 0);
	for (int ki = 0; ki < antialiasing; ++ki) {
		for (int kj = 0; kj < antialiasing; ++kj) {
			double x1 = x + double(ki)/antialiasing;
			double y1 = y + double(kj)/antialiasing;
			Ray ray = camera.getRay(x1, y1, isDiffuse);
			clr += computeColor(ray);
		}
	}
	clr /= antialiasing * antialiasing;
	return clr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
PathTracing::PathTracing(const Camera& camera, 
						 const Object& scene,
						 Image& img, 
						 int samples,
						 int maxDepth,
						 double tMax) : StandardRendererWithPointLight(camera, scene, img, maxDepth, tMax, 1, false), samples(samples) {
}

//-----------------------------------------------------------------------------
Color PathTracing::computePixel(int x, int y) {
	Color clr(0, 0, 0, 0);
	for (int i = 0; i < samples; ++i) {
		double x1 = x + random();
		double y1 = y + random();
		Ray ray = camera.getRay(x1, y1, isDiffuse);
		clr += computeColor(ray);
	}
	clr /= samples;
	return clr;
}

};