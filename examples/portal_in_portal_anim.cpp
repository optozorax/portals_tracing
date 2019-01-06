#include <iostream>
#include <sstream>
#include <omp.h>

#include <windows.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/shape/contour.h>
#include <pt/shape/portals.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/renderer/path-tracing.h>
#include <pt/renderer/thread-renderer.h>
#include <pt/pt2easybmp.h>

using namespace pt;

//-----------------------------------------------------------------------------
class Time
{
public:
	Time() {
		tick = GetTickCount();
	}
	float getTime(void) {
		return (GetTickCount() - tick)/1000.0;
	}
private:
	unsigned long tick;
};

//-----------------------------------------------------------------------------
pt::vec3 transform(pt::CoordSystem c, pt::vec2 b) {
	return c.pos + c.i * b.x + c.j * b.y;
}

//-----------------------------------------------------------------------------
void addContour2(pt::Scene& scene, pt::CoordSystem c, pt::std::vector<vec2> poly, pt::Material* material) {
	using namespace pt;
	double thick = 0.01;
	std::vector<vec3> mas;
	for (int i = 0; i < poly.size(); i++)
		mas.push_back(transform(c, poly[i]));

	scene.array.push_back(new Contour(mas, thick, false, material));
}

//-----------------------------------------------------------------------------
void addPortals(Scene& scene, RayRenderer ren, CoordSystem c1, CoordSystem c2) {
	double it = dot(c1.k, c1.pos - c2.pos)/dot(c1.k, c2.i);
	double jt = dot(c1.k, c1.pos - c2.pos)/dot(c1.k, c2.j);

	std::vector<vec2> mas1; 
	mas1.push_back({it, 0});
	mas1.push_back({1, 0});
	mas1.push_back({1, 1});
	mas1.push_back({0, 1});
	mas1.push_back({0, jt});

	std::vector<vec2> mas2;
	mas2.push_back({0, jt});
	mas2.push_back({0, 0});
	mas2.push_back({it, 0});

	CoordSystem c3 = teleportCoordSystem(c1, c2, c2);

	Scatter* pt1 = new Scatter(pt::Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(pt::Color(0.1, 0.55, 1));

	Portals* prt1 = new Portals(c1, c2, mas1, pt1, pt2);
	Portals* prt2 = new Portals(c1, c3, mas2, pt1, pt2);
	scene.array.push_back(prt1);
	scene.array.push_back(prt2);

	addContour2(scene, c3, mas2, pt2);
	addContour2(scene, c2, mas1, pt2);

	std::vector<vec2> mas3;
	mas3.push_back({0, 0});
	mas3.push_back({1, 0});
	mas3.push_back({1, 1});
	mas3.push_back({0, 1});
	mas3.push_back({0, 0});

	addContour2(scene, c1, mas3, pt1);
}

//-----------------------------------------------------------------------------
void initScene(Scene& scene, RayRenderer& ren, double t) {
	//---------------------------------------------------------------------
	// RAY-RENDERER
	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(-3, 0, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(-0.1, -0.1, 0.1), pt::Color(0.5, 0.5, 0.5)));

	//---------------------------------------------------------------------
	// Комната
	{
		double size, depth;
		//-----------------------------------------------------------------
		// Пол
		size = 5;
		depth = -2;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));

		//-----------------------------------------------------------------
		// Потолок
		size = 5;
		depth = 6;
		vec3 a1(-size, -size, depth);
		vec3 b1(-size, size, depth);
		vec3 c1(size, size, depth);
		vec3 d1(size, -size, depth);
		scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0, 0.1, 0.1))));
		scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0, 0.1, 0.1))));

		//-----------------------------------------------------------------
		// Стена 1
		scene.array.push_back(new Triangle(a, a1, b1, new Scatter(pt::Color(0, 0, 0.1))));
		scene.array.push_back(new Triangle(b1, b, a, new Scatter(pt::Color(0, 0, 0.1))));

		//-----------------------------------------------------------------
		// Стена 2
		scene.array.push_back(new Triangle(b, b1, c1, new Scatter(pt::Color(0, 0.1, 0))));
		scene.array.push_back(new Triangle(c1, c, b, new Scatter(pt::Color(0, 0.1, 0))));

		//-----------------------------------------------------------------
		// Стена 3
		scene.array.push_back(new Triangle(d, c1, d1, new Scatter(pt::Color(0.1, 0, 0))));
		scene.array.push_back(new Triangle(c1, c, d, new Scatter(pt::Color(0.1, 0, 0))));

		//-----------------------------------------------------------------
		// Стена 4
		scene.array.push_back(new Triangle(a, a1, d1, new Scatter(pt::Color(0.1, 0.1, 0))));
		scene.array.push_back(new Triangle(d1, d, a, new Scatter(pt::Color(0.1, 0.1, 0))));
	}

	//---------------------------------------------------------------------
	// Портал в портале
	CoordSystem coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 1, 0);
	coords1.k = vec3(0, 0, -1);
	coords1.pos = vec3(-0.5, -0.5, 0);

	CoordSystem coords2 = coords1;
	coords2 = rotate(coords2, vec3(pt::pi - pt::pi/4, 0, -pt::pi/2));
	coords2.pos += coords1.i * 0.7 + coords1.j * 0.5;
	coords2.pos.z -= t;

	addPortals(scene, ren, coords1, coords2);
}

//-----------------------------------------------------------------------------
int main() {
	using namespace pt;

	#pragma omp parallel for num_threads(4)
	for (int i = 1; i < 301; ++i) {
		Time time;
		Image img(500, 500);
		RayRenderer ren(2);
		Scene scene;

		initScene(scene, ren, i/300.0 * (276.0 / 600.0));

		//---------------------------------------------------------------------
		// Камера
		double alpha = 3.788;
		double beta = 0.334;
		double distance = 1.2;

		vec3 pos(vec3(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta)));

		pos *= distance; 
		pos += vec3(0, 0, 0.5);

		PerspectiveCamera cam(1, pi/2.0, 0, pos, img.getWidth(), img.getHeight());
		cam.lookAt(vec3(0, 0, 0.5));

		//---------------------------------------------------------------------
		// Рендеринг
		ren.render(cam, img, scene);

		img.colorCorrection();

		//---------------------------------------------------------------------
		// Сохранение изображения
		std::stringstream sout;
		sout << "portal_" << i << ".bmp";
		saveAsBmp(img, sout.str());

		std::cout << "i: " << i << ", time: " << time.getTime() << std::endl;
	}
}