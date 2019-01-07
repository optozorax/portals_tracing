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
void addContour2(pt::Scene& scene, crd3 c, std::vector<vec2> poly, pt::Material_ptr material) {
	using namespace pt;
	double thick = 0.01;
	std::vector<vec3> mas;
	for (int i = 0; i < poly.size(); i++)
		mas.push_back(plane3(c).from(poly[i]));

	scene.array.push_back(makeContour(mas, thick, false, material));
}

//-----------------------------------------------------------------------------
void addPortals(Scene& scene, RayTracing ren, crd3 c1, crd3 c2) {
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

	crd3 c3 = space3(c2).from(space3(c1).to(c2));

	Material_ptr pt1 = makeScatter(pt::Color(1, 0.5, 0.15));
	Material_ptr pt2 = makeScatter(pt::Color(0.1, 0.55, 1));

	Portals_ptr prt1 = makePortals(c1, c2, mas1, pt1, pt2);
	Portals_ptr prt2 = makePortals(c1, c3, mas2, pt1, pt2);
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
void initScene(Scene& scene, RayTracing& ren, double t) {
	//---------------------------------------------------------------------
	// RAY-RENDERER
	scene.array.push_back(makeSky(Color(1, 1, 1), Color(1, 1, 1)));
	ren.luminaries.push_back(PointLight(vec3(0.5, 0.5, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLight(vec3(-3, 0, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLight(vec3(-0.1, -0.1, 0.1), pt::Color(0.5, 0.5, 0.5)));

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
		scene.array.push_back(makeTriangle(a, b, c, makeScatter(pt::Color(0.35, 0, 0.35))));
		scene.array.push_back(makeTriangle(c, d, a, makeScatter(pt::Color(0.35, 0, 0.35))));

		//-----------------------------------------------------------------
		// Потолок
		size = 5;
		depth = 6;
		vec3 a1(-size, -size, depth);
		vec3 b1(-size, size, depth);
		vec3 c1(size, size, depth);
		vec3 d1(size, -size, depth);
		scene.array.push_back(makeTriangle(a1, b1, c1, makeScatter(pt::Color(0, 0.35, 0.35))));
		scene.array.push_back(makeTriangle(c1, d1, a1, makeScatter(pt::Color(0, 0.35, 0.35))));

		//-----------------------------------------------------------------
		// Стена 1
		scene.array.push_back(makeTriangle(a, a1, b1, makeScatter(pt::Color(0, 0, 0.35))));
		scene.array.push_back(makeTriangle(b1, b, a, makeScatter(pt::Color(0, 0, 0.35))));

		//-----------------------------------------------------------------
		// Стена 2
		scene.array.push_back(makeTriangle(b, b1, c1, makeScatter(pt::Color(0, 0.35, 0))));
		scene.array.push_back(makeTriangle(c1, c, b, makeScatter(pt::Color(0, 0.35, 0))));

		//-----------------------------------------------------------------
		// Стена 3
		scene.array.push_back(makeTriangle(d, c1, d1, makeScatter(pt::Color(0.35, 0, 0))));
		scene.array.push_back(makeTriangle(c1, c, d, makeScatter(pt::Color(0.35, 0, 0))));

		//-----------------------------------------------------------------
		// Стена 4
		scene.array.push_back(makeTriangle(a, a1, d1, makeScatter(pt::Color(0.35, 0.35, 0))));
		scene.array.push_back(makeTriangle(d1, d, a, makeScatter(pt::Color(0.35, 0.35, 0))));
	}

	//---------------------------------------------------------------------
	// Портал в портале
	crd3 coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 1, 0);
	coords1.k = vec3(0, 0, -1);
	coords1.pos = vec3(-0.5, -0.5, 0);

	crd3 coords2 = coords1;
	coords2 = rotate(coords2, vec3(pt::pi - pt::pi/4, 0, -pt::pi/2));
	coords2.pos += coords1.i * 0.7 + coords1.j * 0.5;
	coords2.pos.z -= t;

	addPortals(scene, ren, coords1, coords2);
}

//-----------------------------------------------------------------------------
int main() {
	using namespace pt;

	for (int i = 1; i < 301; ++i) {
		Time time;
		Image img(500, 500);
		RayTracing ren(2, 4, false);
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
		ren.assign(&cam, &scene, &img);
		ren.render();

		img.colorCorrection();

		//---------------------------------------------------------------------
		// Сохранение изображения
		std::stringstream sout;
		sout << "anim/portal_" << i << ".bmp";
		saveAsBmp(img, sout.str());

		std::cout << "i: " << i << ", time: " << time.getTime() << std::endl;
	}
}