#include <fstream>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/object/cubemap.h>
#include <pt/object/mathroom.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/shape/polygon.h>
#include <pt/shape/cylinder.h>
#include <pt/shape/contour.h>
#include <pt/material/scatter.h>
#include <pt/material/light.h>
#include <pt/material/refract.h>
#include <pt/material/reflect.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/renderer/path-tracing.h>
#include <pt/pt2twg.h>

#include <pt/shape/portals.h>
#include "pt_window.h"

#include <prtl/portal_space.h>
#include <prtl/coord_system.h>
#include <prtl/geometry.h>
#include <prtl/polygon.h>
#include <prtl/animation.h>

using namespace twg;

//-----------------------------------------------------------------------------
class PortalWindow : public pt::PtWindow
{
public:
	PortalWindow(WindowType type);

	void onChangeCam(double alpha, double beta, double distance, int width, int height);
	void createScene(void);	
	void func();

	pt::PerspectiveCamera	cam;
	pt::Scene				scene;
	pt::RayRenderer			ren;
	pt::vec3				pos;

	//PortalSpace sp;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
PortalWindow::PortalWindow(WindowType type) : ren(1, 1000), PtWindow(type, &ren, &cam, &scene), cam(1, pi/2, 0, pt::vec3(0), 1, 1) {}

//-----------------------------------------------------------------------------
void PortalWindow::onChangeCam(double alpha, double beta, double distance, int width, int height) {
	using namespace pt;
	pos = vec3(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta));
	pos *= distance;
	//pos += vec3(0, 0, 0.2);
	//vec3 lookAt(vec3(0, 0, 0.7));
	/*CoordSystem plane = {
		{0.5, -0.5, -0.707106781186548}, 
		{0.5, -0.5, 0.707106781186547}, 
		{-0.707106781186547, -0.707106781186547, -2.22044604925031e-16}, 
		{-0.671751442127221, 0.671751442127221, 0.242893218813453} 
	};
	vec3 lookAt(transform(plane, { 1, 0.65649711574556 }));
	pos = pos + lookAt;*/
	vec3 lookAt(0);
	cam.assign(1, pt::pi/2.0, 0, pos, width, height);
	cam.lookAt(lookAt);
}

//-----------------------------------------------------------------------------
//void PortalWindow::createScene(void) {
//	using namespace pt;
//
//	//-------------------------------------------------------------------------
//	// RAY-RENDERER
//	ren.skyColor = pt::Color(1, 1, 1);
//	ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, -0.5), pt::Color(1.2, 1.2, 1.2)));
//	ren.luminaries.push_back(PointLightSource(vec3(2, -0.5, 2), pt::Color(0.8, 0.8, 0.8)));
//
//	// PATH-RENDERER
//	//scene.array.push_back(new Sky(pt::Color(0.3, 0.3, 0.9), pt::Color(5, 5, 5)));
//
//	//-------------------------------------------------------------------------
//	// Комната
//	{
//		double size, depth;
//		//-------------------------------------------------------------------------
//		// Пол
//		size = 5;
//		depth = -2;
//		vec3 a(-size, -size, depth);
//		vec3 b(-size, size, depth);
//		vec3 c(size, size, depth);
//		vec3 d(size, -size, depth);
//		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
//		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));
//
//		//-------------------------------------------------------------------------
//		// Потолок
//		size = 5;
//		depth = 6;
//		vec3 a1(-size, -size, depth);
//		vec3 b1(-size, size, depth);
//		vec3 c1(size, size, depth);
//		vec3 d1(size, -size, depth);
//		scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0, 0.1, 0.1))));
//		scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0, 0.1, 0.1))));
//
//		//-------------------------------------------------------------------------
//		// Стена 1
//		scene.array.push_back(new Triangle(a, a1, b1, new Scatter(pt::Color(0, 0, 0.1))));
//		scene.array.push_back(new Triangle(b1, b, a, new Scatter(pt::Color(0, 0, 0.1))));
//
//		//-------------------------------------------------------------------------
//		// Стена 2
//		scene.array.push_back(new Triangle(b, b1, c1, new Scatter(pt::Color(0, 0.1, 0))));
//		scene.array.push_back(new Triangle(c1, c, b, new Scatter(pt::Color(0, 0.1, 0))));
//
//		//-------------------------------------------------------------------------
//		// Стена 3
//		scene.array.push_back(new Triangle(d, c1, d1, new Scatter(pt::Color(0.1, 0, 0))));
//		scene.array.push_back(new Triangle(c1, c, d, new Scatter(pt::Color(0.1, 0, 0))));
//
//		//-------------------------------------------------------------------------
//		// Стена 4
//		scene.array.push_back(new Triangle(a, a1, d1, new Scatter(pt::Color(0.1, 0.1, 0))));
//		scene.array.push_back(new Triangle(d1, d, a, new Scatter(pt::Color(0.1, 0.1, 0))));
//	}
//
//	//-------------------------------------------------------------------------
//	// Различные эксперименты
//	//scene.array.push_back(new Cylinder(new Reflect(pt::Color(0.8, 0.8, 0.8), 0)));
//	//scene.array.push_back(new Sphere(new Scatter(pt::Color(0.1, 0.1, 0.1))));
//	//scene.array.push_back(new TrMove(vec3(0, 0, 1), new Sphere(new Refract(1.5, 0))));
//	//scene.array.push_back(new );
//	//scene.array.push_back(new TrRotate(vec3(pt::pi/5, pt::pi/9, 0), new TrScale(vec3(1, 1, 2), new Sphere(new Refract(1.5, 0)))));
//	//scene.array.push_back(new TrComplex(vec3(10, 1, 1), vec3(pt::pi/2, 0, 0), vec3(0, 0, 1), new Sphere(new Refract(1.5, 0))));
//
//	////-------------------------------------------------------------------------
//	//// Квадратные порталы
//	//const double psize = 4;
//	//const double pheight = 5;
//	//a = vec3(psize, psize, -1);
//	//b = vec3(psize, -psize, -1);
//	//c = vec3(psize, -psize, -1 + pheight);
//	//d = vec3(psize, psize, -1 + pheight);
//	//vec3 a1(-psize, psize, -1);
//	//vec3 b1(-psize, -psize, -1);
//	//vec3 c1(-psize, -psize, -1 + pheight);
//	//vec3 d1(-psize, psize, -1 + pheight);
//	//scene.array.push_back(new Portals(a, b, c, a1, b1, c1, pt::Color(0.5, 0.5, 0.5)));
//	//scene.array.push_back(new Portals(a, c, d, a1, c1, d1, pt::Color(0.5, 0.5, 0.5)));
//
//	//std::vector<vec3> array;
//	//array.push_back(a);
//	//array.push_back(b);
//	//array.push_back(c);
//	//array.push_back(d);
//	//scene.array.push_back(new Contour(array, 0.05, new Scatter(pt::Color(0.2, 0.2, 0.6))));
//	//array.erase(array.begin(), array.end());
//	//array.push_back(a1);
//	//array.push_back(b1);
//	//array.push_back(c1);
//	//array.push_back(d1);
//	//scene.array.push_back(new Contour(array, 0.05, new Scatter(pt::Color(0.6, 0.3, 0.2))));
//
//	//-------------------------------------------------------------------------
//	// ПОРТАЛ В ПОРТАЛЕ
//	{
//		vec3 a(0, 0, 0);
//		vec3 b(1, 0, 0);
//		vec3 c(0, 1, 0);
//		vec3 d(b + c);
//
//		double v = 0.56;
//		double x = 0.5;
//		double y = 0.5;
//
//		std::ifstream fin("portal.txt");
//		if (fin.is_open()) {
//			fin >> v >> x >> y;
//			fin.close();
//		}
//
//		vec3 i(b - a);
//		i.normalize();
//		vec3 j(c - a);
//		j.normalize();
//		vec3 k(cross(i, j));
//		k.normalize();
//
//		double l = (b-a).getLength();
//		double sq2 = sqrt(2.0);
//
//		vec3 ab(a + i*v*l);
//		vec3 ac(a + j*v*l);
//
//		vec3 a1(a + (i*x + j*y + k*(-v/sq2)) * l);
//		vec3 ab1(a + (i * x + j * (y - v/sq2)) * l);
//		vec3 ac1(a + (i * x + j * (y + v/sq2)) * l);
//
//		vec3 i1(ab1 - a1);
//		i1.normalize();
//		vec3 j1(ac1 - a1);
//		j1.normalize();
//		vec3 k1(cross(i1, j1));
//		k1.normalize();
//
//		vec3 b1(a1 + i1 * l);
//		vec3 c1(a1 + j1 * l);
//		vec3 d1(a1 + i1 * l + j1 * l);
//
//		vec3 ab2(a1 + (i1 * x + j1 * (y - v/sq2)) * l);
//		vec3 ac2(a1 + (i1 * x + j1 * (y + v/sq2)) * l);
//		vec3 a2(a1 + (i1 * x + j1 * y - k1 * v/sq2) * l);
//		
//		scene.array.push_back(new Portals(a, ab, ac, a2, ab2, ac2, pt::Color(0.5, 0.5, 0.5)));
//		scene.array.push_back(new Portals(d, c, b, d1, c1, b1, pt::Color(0.5, 0.5, 0.5)));
//		scene.array.push_back(new Portals(ab, b, ac, ab1, b1, ac1, pt::Color(0.5, 0.5, 0.5)));
//		scene.array.push_back(new Portals(ac, b, c, ac1, b1, c1, pt::Color(0.5, 0.5, 0.5)));
//	
//		// Подложка под порталы, чтобы было видно какой портал синий, какой оранжевый
//		double p = 0.02;
//		double p1 = 0.001;
//		vec3 ap(a - i*p - j*p - k*p1);
//		vec3 bp(b + i*p - j*p - k*p1);
//		vec3 cp(c - i*p + j*p - k*p1);
//		vec3 dp(d + i*p + j*p - k*p1);
//		vec3 ab1p(ab1 - j*p*sq2 + k1*p1);
//		vec3 ac1p(ac1 + j*p*sq2 + k1*p1);
//		vec3 b1p(b1 + i1*p - j1*p + k1*p1);
//		vec3 c1p(c1 - i1*p + j1*p + k1*p1);
//		vec3 d1p(d1 + i1*p + j1*p + k1*p1);
//		vec3 ab2p(ab2 - j1*p*sq2 - i1*p1);
//		vec3 ac2p(ac2 + j1*p*sq2 - i1*p1);
//		vec3 a2p(a2 - k1*p*sq2 - i1*p1);
//
//		pt::Color pclr1(1, 0.5, 0.15);
//		pt::Color pclr2(0.1, 0.55, 1);
//		scene.array.push_back(new Triangle(ap, bp, cp, new Scatter(pclr1)));
//		scene.array.push_back(new Triangle(cp, bp, dp, new Scatter(pclr1)));
//		scene.array.push_back(new Triangle(ac1p, ab1p, b1p, new Scatter(pclr2)));
//		scene.array.push_back(new Triangle(ac1p, b1p, c1p, new Scatter(pclr2)));
//		scene.array.push_back(new Triangle(b1p, d1p, c1p, new Scatter(pclr2)));
//		scene.array.push_back(new Triangle(ab2p, ac2p, a2p, new Scatter(pclr2)));
//
//		// Полигоны, показывающие как располагаются части порталов
//		/*const vec3 offset(0, 0, 0);
//		pt::Color clr1(0.1, 0.1, 0.1);
//		pt::Color clr2(0.2, 0.2, 0.2);
//		scene.array.push_back(new Triangle(a + offset, ab + offset, ac + offset, new Scatter(clr1)));
//		scene.array.push_back(new Triangle(a2 + offset, ab2 + offset, ac2 + offset, new Scatter(clr2)));
//
//		pt::Color clr3(0.3, 0.3, 0.3);
//		pt::Color clr4(0.4, 0.4, 0.4);
//		scene.array.push_back(new Triangle(d + offset, c + offset, b + offset, new Scatter(clr3)));
//		scene.array.push_back(new Triangle(d1 + offset, c1 + offset, b1 + offset, new Scatter(clr4)));
//		
//		scene.array.push_back(new Triangle(ab + offset, b + offset, ac + offset, new Scatter(clr3)));
//		scene.array.push_back(new Triangle(ab1 + offset, b1 + offset, ac1 + offset, new Scatter(clr4)));
//		
//		scene.array.push_back(new Triangle(ac + offset, b + offset, c + offset, new Scatter(clr3)));
//		scene.array.push_back(new Triangle(ac1 + offset, b1 + offset, c1 + offset, new Scatter(clr4)));*/
//	}
//
//
//
//	//using namespace pt;
//	////ren.skyColor = pt::Color(1, 1, 1);
//	////ren.luminaries.push_back(PointLightSource(vec3(0, 0, 3), pt::Color(1.2, 1.2, 1.2)));
//	////ren.luminaries.push_back(PointLightSource(vec3(0, 1, 3), pt::Color(1.2, 1.2, 1.2)));
//	//scene.array.push_back(new Sky(pt::Color(0.3, 0.3, 0.9), pt::Color(5, 5, 5)));
//
//	//// Пол
//	//const double size = 2;
//	//const double depth = size/2;
//	//vec3 a(-size, -size, -depth);
//	//vec3 b(-size, size, -depth);
//	//vec3 c(size, size, -depth);
//	//vec3 d(size, -size, -depth);
//	//vec3 a1(-size, -size, depth);
//	//vec3 b1(-size, size, depth);
//	//vec3 c1(size, size, depth);
//	//vec3 d1(size, -size, depth);
//	//scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));
//	////scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0.1, 0, 0.1))));
//	////scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(a, a1, b, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(b, b1, a1, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(b, b1, c, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(c, c1, b1, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(c, c1, d, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(d, d1, c1, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(a, a1, d, new Scatter(pt::Color(0.1, 0, 0.1))));
//	//scene.array.push_back(new Triangle(d, d1, a1, new Scatter(pt::Color(0.1, 0, 0.1))));
//
//	//// Сфера
//	//scene.array.push_back(new Sphere(new Scatter(pt::Color(0.1, 0.1, 0.1, 0.5))));
//	////scene.array.push_back(new TrComplex(vec3(1), vec3(0), vec3(0, 0, 3), new Sphere(new Light(pt::Color(40, 40, 40)))));
//
//	//// Треугольник
//	//a = vec3(0, 0, 1);
//	//b = vec3(0.5, 1.2, 1);
//	//c = vec3(1.2, 1.2, 1.2);
//	//scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0.1, 0.2, 0.5))));
//	//a = vec3(0, 0, 0.8);
//	//b = vec3(0.5, 1.2, 0.8);
//	//c = vec3(1.2, 1.2, 1);
//	//scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(1, 0, 0, 0.99))));
//}

std::vector<pt::vec2> prtl2pt(const std::vector<prtl::vec2>& a);
pt::vec2 prtl2pt(const prtl::vec2& a);
pt::vec3 prtl2pt(const prtl::vec3& a);
pt::CoordSystem prtl2pt(const prtl::CoordSystem& a);

std::vector<prtl::vec2> pt2prtl(const std::vector<pt::vec2>& a);
prtl::vec2 pt2prtl(const pt::vec2& a);
prtl::vec3 pt2prtl(const pt::vec3& a);
prtl::CoordSystem pt2prtl(const pt::CoordSystem& a);

std::vector<pt::vec2> prtl2pt(const std::vector<prtl::vec2>& a) {
	std::vector<pt::vec2> result;
	for (const auto& i : a)
		result.push_back(prtl2pt(i));
	return result;
}

pt::vec2 prtl2pt(const prtl::vec2& a) {
	return pt::vec2(a.x, a.y);
}

pt::vec3 prtl2pt(const prtl::vec3& a) {
	return pt::vec3(a.x, a.y, a.z);
}

pt::CoordSystem prtl2pt(const prtl::CoordSystem& a) {
	return {prtl2pt(a.i), prtl2pt(a.j), prtl2pt(a.k), prtl2pt(a.pos),};
}

std::vector<prtl::vec2> pt2prtl(const std::vector<pt::vec2>& a) {
	std::vector<prtl::vec2> result;
	for (const auto& i : a)
		result.push_back(pt2prtl(i));
	return result;
}

prtl::vec2 pt2prtl(const pt::vec2& a) {
	return prtl::vec2(a.x, a.y);
}

prtl::vec3 pt2prtl(const pt::vec3& a) {
	return prtl::vec3(a.x, a.y, a.z);
}

prtl::CoordSystem pt2prtl(const pt::CoordSystem& a) {
	return {pt2prtl(a.i), pt2prtl(a.j), pt2prtl(a.k), pt2prtl(a.pos)};
}

void addCoordSys(pt::Scene& scene, pt::CoordSystem c) {
	using namespace pt;
	Scatter* sc1 = new Scatter(pt::Color(0.3, 0.3, 0.7));
	Scatter* sc2 = new Scatter(pt::Color(0.3, 0.7, 0.3));
	Scatter* sc3 = new Scatter(pt::Color(0.7, 0.3, 0.3));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.i, 0.02, sc1));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.j, 0.02, sc2));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.k, 0.02, sc3));
}

//-----------------------------------------------------------------------------
pt::vec2 findBisec(const std::vector<prtl::vec2> polygon, int i, double h) {
	pt::vec2 a, b, c;
	if (i == 0) {
		a = polygon.back();
		b = polygon[i];
		c = polygon[i+1];
	} else if (i == polygon.size() - 1) {
		a = polygon[i-1];
		b = polygon[i];
		c = polygon[0];
	} else {
		a = polygon[i-1];
		b = polygon[i];
		c = polygon[i+1];
	}

	pt::vec2 first = a - b;
	pt::vec2 second = c - b;
	first.normalize();
	second.normalize();

	pt::vec2 bisec = first + second;
	bisec.normalize();

	if (prtl::pointInPolygon(pt2prtl(polygon), pt2prtl(b + bisec * h)))
		bisec = -bisec;

	return bisec;
}

//-----------------------------------------------------------------------------
std::vector<prtl::vec2> createFirstContour(const std::vector<prtl::vec2>& portal, double h) {
	std::vector<prtl::vec2> result;
	result.push_back(portal[0]);
	for (int i = 0; i < portal.size(); i++)
		result.push_back(portal[i] + findBisec(portal, i, h) * h);

	for (int i = portal.size() - 1; i > 0; i--)
		result.push_back(portal[i]);

	return result;
}

//-----------------------------------------------------------------------------
std::vector<prtl::vec2> createSecondContour(const std::vector<prtl::vec2>& portal, double h) {
	std::vector<prtl::vec2> result;

	int end = portal.size() - 1;
	result.push_back(portal[0]);
	result.push_back(portal[0] + findBisec(portal, 0, h) * h);
	result.push_back(portal[end] + findBisec(portal, end, h) * h);
	result.push_back(portal[end]);

	return result;
}


/*void PortalWindow::createScene(void) {
	using namespace pt;

	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 5), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(5, 5, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(-5, -3, 3), pt::Color(0.5, 0.5, 0.5)));
	ren.skyColor = pt::Color(1, 1, 1);

	Scatter* sc1 = new Scatter(pt::Color(0.5, 0.5, 0.9));
	Scatter* sc2 = new Scatter(pt::Color(0.6, 0.6, 0.6));
	Scatter* sc3 = new Scatter(pt::Color(0.9, 0.5, 0.5));

	{
		const double size = 50;
		const double depth = -0.5;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.4, 0, 0.4))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.4, 0, 0.4))));
	}

	std::vector<vec2> pPoly = {
		{1, 0.65649711574556},
		{1, 0.671751442127222},
		{1.01525432638166, 0.671751442127222}
	};

	CoordSystem cPoly = {
		{0.5, -0.5, -0.707106781186548}, 
		{0.5, -0.5, 0.707106781186547}, 
		{-0.707106781186547, -0.707106781186547, -2.22044604925031e-16}, 
		{-0.671751442127221, 0.671751442127221, 0.242893218813453} 
	};

	std::vector<vec2> pPoly1 = {
		{0, 1},
		{0.319932658214892, 1},
		{0.831717771234668, 0.488214886980224},
		{0.343502884254443, 0},
		{0, 0}
	};

	CoordSystem cPoly1 = {
		{1, 0, 0}, 
		{0, 1, 0}, 
		{0, 0, 1}, 
		{-0.5, -0.5, 0} 
	};

	vec3 move = {0.035355339059327383, -0.035355339059327369, -0.050000000000000003};

	scene.array.push_back(new pt::Polygon(pPoly, cPoly, sc1));
	scene.array.push_back(new pt::Polygon(pPoly1, cPoly1, sc2));
	cPoly.pos += move / 3.0;
	scene.array.push_back(new pt::Polygon(pPoly, cPoly, sc3));
}*/

/*pt::vec3 transform(pt::CoordSystem c, pt::vec2 b) {
	return c.pos + c.i * b.x + c.j * b.y;
}*/

void addContour(pt::Scene& scene, pt::CoordSystem c, std::vector<prtl::vec2> poly, pt::Material* material, double thick) {
	using namespace pt;
	//double thick = 0.02;
	poly.push_back(poly[0]);
	poly.push_back(poly[poly.size() - 2]);
	std::vector<vec3> mas;
	for (int i = 1; i < poly.size() - 1; i++) {
		vec3 a = transform(c, poly[i - 1]) - transform(c, poly[i]);
		vec3 b = transform(c, poly[i + 1]) - transform(c, poly[i]);
		vec3 dir = -(a + b);
		dir.normalize();
		mas.push_back(transform(c, poly[i]) + dir * thick);
	}

	scene.array.push_back(new Contour(mas, thick, false, material));
}

void addContour2(pt::Scene& scene, pt::CoordSystem c, std::vector<prtl::vec2> poly, pt::Material* material, double thick) {
	using namespace pt;
	//double thick = 0.01;
	std::vector<vec3> mas;
	for (int i = 0; i < poly.size(); i++)
		mas.push_back(transform(c, poly[i]));

	scene.array.push_back(new Contour(mas, thick, false, material));
}

/*void PortalWindow::createScene(void) {
	using namespace pt;

	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 4), pt::Color(1, 1, 1)));
	ren.skyColor = pt::Color(0.5, 0.5, 0.5);

	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.7, 0.5));
	Scatter* sc2 = new Scatter(pt::Color(0.1, 0.1, 0.1));
	Reflect* rft = new Reflect(pt::Color(1, 1, 1), 0);

	Scatter* pt1 = new Scatter(pt::Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(pt::Color(0.1, 0.55, 1));

	{
		const double size = 50;
		const double depth = -0.5;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));
	}

	CoordSystem coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 0, 1);
	coords1.k = vec3(0, 1, 0);
	coords1.pos = vec3(-0.5, 0.5, 2);
	coords1 = rotate(coords1, vec3(0, -pt::pi/2, 0));

	{
		std::vector<vec2> mas2;
		mas2.push_back({0, 0});
		mas2.push_back({0, 1});
		mas2.push_back({1, 1});
		mas2.push_back({1, 0});

		CoordSystem coords2;
		coords2 = coords1;
		coords2.pos = vec3(-0.5, 0.5, 1.8);

		Portals* prt = new Portals(coords2, coords1, mas2, pt1, pt2);
		scene.array.push_back(prt);
		ren.portals.push_back(prt);
		addContour(scene, coords2, mas2, pt1);
		addContour(scene, coords1, mas2, pt2);

		coords1.pos = vec3(-0.5, 0.5, 1.4);
		coords2.pos = vec3(-0.5, 0.5, 1);

		Portals* prt1 = new Portals(coords2, coords1, mas2, pt1, pt2);
		scene.array.push_back(prt1);
		ren.portals.push_back(prt1);
		addContour(scene, coords2, mas2, pt1);
		addContour(scene, coords1, mas2, pt2);

		coords2.pos = vec3(-0.5, 0.5, 0.5);
		scene.array.push_back(new pt::Polygon(mas2, coords2, sc));

		coords1.pos = vec3(-0.5, 0.5, 3);
		scene.array.push_back(new pt::Polygon(mas2, coords1, sc));

		coords1.pos = vec3(-0.5, 0.5, 1.6);
		scene.array.push_back(new pt::Polygon(mas2, coords1, sc));
	}
}*/

/*void PortalWindow::createScene(void) {
	using namespace pt;

	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 4), pt::Color(1, 1, 1)));
	ren.skyColor = pt::Color(0.5, 0.5, 0.5);

	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.7, 0.5));
	Scatter* sc2 = new Scatter(pt::Color(0.1, 0.1, 0.1));
	Reflect* rft = new Reflect(pt::Color(1, 1, 1), 0);

	Scatter* pt1 = new Scatter(pt::Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(pt::Color(0.1, 0.55, 1));

	{
		const double size = 50;
		const double depth = -0.5;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));
	}
	
	CoordSystem coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 1, 0);
	coords1.k = vec3(0, 0, 1);
	coords1.pos = vec3(-0.5, 0.5, 0);

	CoordSystem coords2 = coords1;
	coords2 = rotate(coords2, vec3(3*pt::pi/4, 0, -pt::pi/2));
	coords2.pos += coords1.i * 0.5 + coords1.j * 0.5;
	coords2.pos.z -= 0.2;

	addCoordSys(scene, coords1);
	addCoordSys(scene, coords2);
}*/

void addPortals(pt::Scene& scene, pt::RayRenderer ren, pt::CoordSystem c1, pt::CoordSystem c2, double t) {
	using namespace pt;
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

	addContour2(scene, c3, mas2, pt2, 0.01);
	addContour2(scene, c2, mas1, pt2, 0.01);

	std::vector<vec2> mas3;
	mas3.push_back({0, 0});
	mas3.push_back({1, 0});
	mas3.push_back({1, 1});
	mas3.push_back({0, 1});
	mas3.push_back({0, 0});

	addContour2(scene, c1, mas3, pt1, 0.01);

	//scene.array.push_back(new pt::Polygon(mas1, c1, pt1));
	//scene.array.push_back(new pt::Polygon(mas1, c2, pt2));

	//scene.array.push_back(new pt::Polygon(mas2, c1, pt1));
	//scene.array.push_back(new pt::Polygon(mas2, c3, pt2));

	//ren.portals.push_back(prt1);
	//ren.portals.push_back(prt2);
}

/*void PortalWindow::createScene(void) {
	using namespace pt;
	ren.skyColor = pt::Color(1, 1, 1);
	//ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, -0.5), pt::Color(1.2, 1.2, 1.2)));
	//ren.luminaries.push_back(PointLightSource(vec3(2, -0.5, 2), pt::Color(0.8, 0.8, 0.8)));
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

	CoordSystem coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 1, 0);
	coords1.k = vec3(0, 0, -1);
	coords1.pos = vec3(-0.5, -0.5, 0);

	CoordSystem coords2 = coords1;
	coords2 = rotate(coords2, vec3(pt::pi - pt::pi/4, 0, -pt::pi/2 + 0.2));
	coords2.pos += coords1.i * 0.7 + coords1.j * 0.5;
	coords2.pos.z -= 0.4;

	addPortals(scene, ren, coords1, coords2, 0.5);
}*/

/*void PortalWindow::createScene(void) {
	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(pt::PointLightSource(vec3(0.5, 0.5, 3), pt::Color(0.5, 0.5, 0.5)));

	std::vector<vec2> poly;
	poly.push_back({0, 0});
	poly.push_back({-0.1, 0.5});
	poly.push_back({0.1, 0.5});

	CoordSystem pPoly;
	pPoly.pos = vec3(0.8, 1, 0.6);
	pPoly.i = vec3(1, 0, 0);
	pPoly.j = vec3(0, 1, 0);
	pPoly.k = vec3(0, 0, 1);
	pPoly = rotate(pPoly, vec3(-0.55, 0, 0));

	int id = sp.addPolygon(poly, pPoly);

	CoordSystem c1;
	c1.i = vec3(1, 0, 0);
	c1.j = vec3(0, 1, 0);
	c1.k = vec3(0, 0, 1);
	c1.pos = vec3(0, 0, 0);

	CoordSystem c2 = c1;
	c2 = rotate(c2, vec3(pt::pi - pt::pi/4, 0, -pt::pi/2));
	c2.pos += c1.i * 0.5 + c1.j * 0.7;
	c2.pos.z -= 0.3;
	c2.k = -c2.k;

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

	sp.addPortals(mas1, c1, c2);
	sp.addPortals(mas2, c1, c3);

	func();
}*/

void addFloor(pt::Scene& scene, double size, double depth, pt::Color clr) {
	using namespace pt;
	vec3 a(-size, -size, depth);
	vec3 b(-size, size, depth);
	vec3 c(size, size, depth);
	vec3 d(size, -size, depth);
	scene.array.push_back(new pt::Triangle(a, b, c, new pt::Scatter(clr)));
	scene.array.push_back(new pt::Triangle(c, d, a, new pt::Scatter(clr)));
}

/*void PortalWindow::func() {
	using namespace pt;
	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.5));
	Scatter* sc2 = new Scatter(pt::Color(0, 0, 0.5));

	CoordSystem pPoly;
	pPoly.pos = vec3(0.8, 1, 0.6);
	pPoly.i = vec3(1, 0, 0);
	pPoly.j = vec3(0, 1, 0);
	pPoly.k = vec3(0, 0, 1);
	pPoly = rotate(pPoly, vec3(-0.55, 0, 0));

	sp.movePolygon(id, pPoly.j * (-0.05));

	scene.array.clear();
	addFloor(scene, 5, -0.5, pt::Color(0.2, 0.2, 0.2));

	for (int i = 0; i < sp.getPolygonCount(); ++i) {
		Polygons polys = sp.getPolygon(sp.getPolygonId(i));
		for (auto& j : polys)
			scene.array.push_back(new pt::Polygon(j.poly, j.p, sc));
	}

	for (int i = 0; i < sp.getPortalCount(); ++i) {
		auto prt = sp.getPortal(sp.getPortalId(i));
		prt.p1.k = -prt.p1.k;
		prt.p2.k = -prt.p2.k;
		Portals* prr = new Portals(prt.p1, prt.p2, prt.poly, sc2, sc2);
		//scene.array.push_back(prr);
		scene.array.push_back(new pt::Polygon(prt.poly, prt.p1, sc2));
		scene.array.push_back(new pt::Polygon(prt.poly, prt.p2, sc2));
		//m_ren.portals.push_back(prr);
		//addContour2(scene, prt.p1, prt.poly, sc2);
		//addContour2(scene, prt.p2, prt.poly, sc2);
	}
}*/

/*void PortalWindow::createScene(void) {
	using namespace pt;

	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, -0.5), pt::Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(vec3(2, -0.5, 2), pt::Color(0.8, 0.8, 0.8)));

	//-------------------------------------------------------------------------
	// Комната
	{
		double size, depth;
		//---------------------------------------------------------------------
		// Пол
		size = 5;
		depth = -2;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));

		//---------------------------------------------------------------------
		// Потолок
		size = 5;
		depth = 6;
		vec3 a1(-size, -size, depth);
		vec3 b1(-size, size, depth);
		vec3 c1(size, size, depth);
		vec3 d1(size, -size, depth);
		scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0, 0.1, 0.1))));
		scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0, 0.1, 0.1))));

		//---------------------------------------------------------------------
		// Стена 1
		scene.array.push_back(new Triangle(a, a1, b1, new Scatter(pt::Color(0, 0, 0.1))));
		scene.array.push_back(new Triangle(b1, b, a, new Scatter(pt::Color(0, 0, 0.1))));

		//---------------------------------------------------------------------
		// Стена 2
		scene.array.push_back(new Triangle(b, b1, c1, new Scatter(pt::Color(0, 0.1, 0))));
		scene.array.push_back(new Triangle(c1, c, b, new Scatter(pt::Color(0, 0.1, 0))));

		//---------------------------------------------------------------------
		// Стена 3
		scene.array.push_back(new Triangle(d, c1, d1, new Scatter(pt::Color(0.1, 0, 0))));
		scene.array.push_back(new Triangle(c1, c, d, new Scatter(pt::Color(0.1, 0, 0))));

		//---------------------------------------------------------------------
		// Стена 4
		scene.array.push_back(new Triangle(a, a1, d1, new Scatter(pt::Color(0.1, 0.1, 0))));
		scene.array.push_back(new Triangle(d1, d, a, new Scatter(pt::Color(0.1, 0.1, 0))));
	}

	Scatter* sc = new Scatter(pt::Color(0.1, 0.1, 0));

	vec3 a1(0, -1, 0);
	vec3 a2(0, 1, 0);
	double r = 0.5;
	double h = 1;
	int n = 50;
	double l = (vec3(cos(0), sin(0), 0) * r - vec3(cos(pt::pi/n), sin(pt::pi/n), 0) * r).getLength();
	std::vector<vec2> poly;
	poly.push_back({0, 0});
	poly.push_back({l, 0});
	poly.push_back({l, h});
	poly.push_back({0, h});
	for (int i = 0; i < n; ++i) {
		double alpha = pt::pi/n * i;
		double beta = pt::pi/n * (i+1);
		vec3 a = a1 + vec3(cos(alpha), sin(alpha), 0) * r;
		vec3 b = a1 + vec3(cos(beta), sin(beta), 0) * r;
		vec3 c = a + vec3(0, 0, 1);

		CoordSystem b1 = calculateCoordSystem(a, b, c);
		vec3 a3 = a2 + vec3(cos(alpha), -sin(alpha), 0) * r;
		vec3 b3 = a2 + vec3(cos(beta), -sin(beta), 0) * r;
		vec3 c3 = a3 + vec3(0, 0, 1);
		CoordSystem b2 = calculateCoordSystem(a3, b3, c3);
		scene.array.push_back(new Portals(b1, b2, poly, sc, sc));
	}
}*/

/*void PortalWindow::createScene(void) {
	// Triple portal
	using namespace pt;

	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 4), pt::Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, -0.5), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(2, -0.5, 2), pt::Color(0.8, 0.8, 0.8)));

	//-------------------------------------------------------------------------
	// Комната
	{
		double size, depth;
		//---------------------------------------------------------------------
		// Пол
		size = 5;
		depth = -2;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));

		//---------------------------------------------------------------------
		// Потолок
		size = 5;
		depth = 6;
		vec3 a1(-size, -size, depth);
		vec3 b1(-size, size, depth);
		vec3 c1(size, size, depth);
		vec3 d1(size, -size, depth);
		scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0, 0.1, 0.1))));
		scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0, 0.1, 0.1))));

		//---------------------------------------------------------------------
		// Стена 1
		scene.array.push_back(new Triangle(a, a1, b1, new Scatter(pt::Color(0, 0, 0.1))));
		scene.array.push_back(new Triangle(b1, b, a, new Scatter(pt::Color(0, 0, 0.1))));

		//---------------------------------------------------------------------
		// Стена 2
		scene.array.push_back(new Triangle(b, b1, c1, new Scatter(pt::Color(0, 0.1, 0))));
		scene.array.push_back(new Triangle(c1, c, b, new Scatter(pt::Color(0, 0.1, 0))));

		//---------------------------------------------------------------------
		// Стена 3
		scene.array.push_back(new Triangle(d, c1, d1, new Scatter(pt::Color(0.1, 0, 0))));
		scene.array.push_back(new Triangle(c1, c, d, new Scatter(pt::Color(0.1, 0, 0))));

		//---------------------------------------------------------------------
		// Стена 4
		scene.array.push_back(new Triangle(a, a1, d1, new Scatter(pt::Color(0.1, 0.1, 0))));
		scene.array.push_back(new Triangle(d1, d, a, new Scatter(pt::Color(0.1, 0.1, 0))));
	}

	Scatter* sc1 = new Scatter(pt::Color(0, 0.3, 0));
	Scatter* sc2 = sc1; //new Scatter(pt::Color(0.1, 0, 0.1));
	Scatter* sc3 = sc1; //new Scatter(pt::Color(0, 0.1, 0.1));

	double thick = 0.02;
	double len = 1;

	std::vector<vec2> poly;
	poly.push_back({0, len});
	poly.push_back({0, 0});
	poly.push_back({len, 0});
	poly.push_back({len, len});

	std::vector<vec2> poly1;
	poly1.push_back({len, 0});
	poly1.push_back({len, len});
	poly1.push_back({0, len});
	poly1.push_back({0, 0});

	CoordSystem cord = standard;
	cord.j = standard.k;
	cord.k = standard.j;

	vec3 pos1(1, 0, 0);
	vec3 pos2(-sin(pt::pi/6.0), cos(pt::pi/6.0), 0);
	vec3 pos3(-sin(pt::pi/6.0), -cos(pt::pi/6.0), 0);

	{
		CoordSystem a1 = cord;
		CoordSystem a2 = cord;
		a1.pos = pos1;
		a2.pos = pos2;
		a1 = rotate(a1, vec3(pt::pi/3, 0, 0));
		a2 = rotate(a2, vec3(pt::pi/3, 0, 0));
		Portals* p1 = new Portals(a1, a2, poly, sc3, sc3);
		scene.array.push_back(p1);
		ren.portals.push_back(p1);
		addContour(scene, a1, poly, sc1, thick);
		addContour(scene, a2, poly, sc1, thick);
	}

	{
		CoordSystem a1 = cord;
		CoordSystem a2 = cord;
		double angle = -pt::pi/3;
		vec3 offset(cos(angle), sin(angle), 0);
		offset *= len;
		a1.pos = pos1 + offset;
		a2.pos = pos3 + offset;

		double angle2 = pt::pi/6 + pt::pi/2;
		a1 = rotate(a1, vec3(angle2, 0, 0));
		a2 = rotate(a2, vec3(angle2, 0, 0));
		Portals* p1 = new Portals(a1, a2, poly, sc3, sc3);
		scene.array.push_back(p1);
		ren.portals.push_back(p1);
		addContour(scene, a1, poly1, sc1, thick);
		addContour(scene, a2, poly1, sc1, thick);
	}

	{
		CoordSystem a1 = cord;
		CoordSystem a2 = cord;
		a1.pos = pos3 + vec3(-len, 0, 0);
		a2.pos = pos2 + vec3(-len, 0, 0);
		Portals* p1 = new Portals(a1, a2, poly, sc3, sc3);
		scene.array.push_back(p1);
		ren.portals.push_back(p1);
		addContour(scene, a1, poly1, sc1, thick);
		addContour(scene, a2, poly1, sc1, thick);
	}

	//addCoordSys(scene, standard);

	double dist = 0.8;

	Scatter* sc4 = new Scatter(pt::Color(0.1, 0.2, 0.3));
	Scatter* sc5 = new Scatter(pt::Color(0.3, 0.2, 0.1));
	Scatter* sc6 = new Scatter(pt::Color(0.2, 0.3, 0.1));
	scene.array.push_back(new Sphere(pos2 * (1+dist) + vec3(0, 0, len/2.0), 0.2, sc4));
	scene.array.push_back(new Sphere(pos1 * (1+dist) + vec3(0, 0, len/2.0), 0.2, sc5));
	scene.array.push_back(new Sphere(pos3 * (1+dist) + vec3(0, 0, len/2.0), 0.2, sc6));
}*/

/*void PortalWindow::createScene(void) {
	// Triple portal
	using namespace pt;

	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 4), pt::Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(vec3(0.5, 0.5, -0.5), pt::Color(0.5, 0.5, 0.5)));
	ren.luminaries.push_back(PointLightSource(vec3(2, -0.5, 2), pt::Color(0.8, 0.8, 0.8)));

	//-------------------------------------------------------------------------
	// Комната
	{
	double size, depth;
	//---------------------------------------------------------------------
	// Пол
	size = 5;
	depth = -2;
	vec3 a(-size, -size, depth);
	vec3 b(-size, size, depth);
	vec3 c(size, size, depth);
	vec3 d(size, -size, depth);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
	scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));

	//---------------------------------------------------------------------
	// Потолок
	size = 5;
	depth = 6;
	vec3 a1(-size, -size, depth);
	vec3 b1(-size, size, depth);
	vec3 c1(size, size, depth);
	vec3 d1(size, -size, depth);
	scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0, 0.1, 0.1))));
	scene.array.push_back(new Triangle(c1, d1, a1, new Scatter(pt::Color(0, 0.1, 0.1))));

	//---------------------------------------------------------------------
	// Стена 1
	scene.array.push_back(new Triangle(a, a1, b1, new Scatter(pt::Color(0, 0, 0.1))));
	scene.array.push_back(new Triangle(b1, b, a, new Scatter(pt::Color(0, 0, 0.1))));

	//---------------------------------------------------------------------
	// Стена 2
	scene.array.push_back(new Triangle(b, b1, c1, new Scatter(pt::Color(0, 0.1, 0))));
	scene.array.push_back(new Triangle(c1, c, b, new Scatter(pt::Color(0, 0.1, 0))));

	//---------------------------------------------------------------------
	// Стена 3
	scene.array.push_back(new Triangle(d, c1, d1, new Scatter(pt::Color(0.1, 0, 0))));
	scene.array.push_back(new Triangle(c1, c, d, new Scatter(pt::Color(0.1, 0, 0))));

	//---------------------------------------------------------------------
	// Стена 4
	scene.array.push_back(new Triangle(a, a1, d1, new Scatter(pt::Color(0.1, 0.1, 0))));
	scene.array.push_back(new Triangle(d1, d, a, new Scatter(pt::Color(0.1, 0.1, 0))));
	}

	Scatter* sc1 = new Scatter(pt::Color(0, 0.3, 0));

	double thick = 0.02;
	double r = 2;
	int n = 6;
	double h = 1;
	double size = 2*r*sin(pt::pi/n);

	std::vector<vec2> poly;
	poly.push_back({0, 0});
	poly.push_back({size, 0});
	poly.push_back({size, h});
	poly.push_back({0, h});

	std::vector<vec2> poly2;
	for (int i = 0; i < n/2; i++) {
		CoordSystem cord = standard;
		cord.j = standard.k;
		cord.k = standard.j;

		vec3 a = vec3(r * sin(2.0 * pt::pi/n * i), r * cos(2.0 * pt::pi/n * i), 0);
		vec3 b = vec3(r * sin(2.0 * pt::pi/n * (i+1)), r * cos(2.0 * pt::pi/n * (i+1)), 0);
		vec3 c = a + vec3(0, 0, 1);
		
		CoordSystem cc1 = calculateCoordSystem(a, b, c);

		vec3 a1 = vec3(r * sin(2.0 * pt::pi/n * (i+1+n/2)), r * cos(2.0 * pt::pi/n * (i+1+n/2)), 0);
		vec3 b1 = vec3(r * sin(2.0 * pt::pi/n * (i+n/2)), r * cos(2.0 * pt::pi/n * (i+n/2)), 0);
		vec3 c1 = a1 + vec3(0, 0, 1);

		CoordSystem cc2 = calculateCoordSystem(a1, b1, c1);
		//cc2.k = -cc2.k;

		scene.array.push_back(new Portals(cc1, cc2, poly, sc1, sc1));
	}

	for (int i = 0; i < n+1; ++i)
		poly2.push_back({r * sin(2.0 * pt::pi/n * i), r * cos(2.0 * pt::pi/n * i)});

	//addCoordSys(scene, standard);

	addContour2(scene, standard, poly2, sc1, thick);
	CoordSystem cord = standard;
	cord.pos.z = h;
	addContour2(scene, cord, poly2, sc1, thick);
}*/

/*void PortalWindow::createScene(void) {
	using namespace pt;
	prtl::PortalSpace sp;

	std::vector<vec2> portalPoly;
	double r1 = 0.5;
	double r2 = 1;
	double n = 100;
	for (int i = 0; i < n; i++)
		portalPoly.push_back(vec2(r1 * cos(2*pt::pi/n*i), r2 * sin(2*pt::pi/n*i)));

	double h = 0.03;
	std::vector<vec2> portalContour = createFirstContour(portalPoly, h);
	std::vector<vec2> portalContour2 = createSecondContour(portalPoly, h);

	pt::CoordSystem coords1;
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 1, 0);
	coords1.k = vec3(0, 0, 1);
	coords1.pos = vec3(-0.5, -0.5, 0.0001);

	pt::CoordSystem coords2 = coords1;
	coords2 = rotate(coords2, vec3(pt::pi - pt::pi/2, 0, pt::pi/2));
	//coords2.pos += coords1.i * 0.5 + coords1.j * 0.5;
	coords2.pos.z += 1.5;

	//addCoordSys(scene, coords2);
	//addCoordSys(scene, standard);
	coords1.pos.x -= coords2.pos.x;
	coords1.pos.y -= coords2.pos.y;
	coords2.pos.x = 0;
	coords2.pos.y = 0;
	coords2 = prtl2pt(prtl::rotateAroundAxis(pt2prtl(coords2), prtl::vec3(0), prtl::vec3(0, 0, 1), pt::pi/4));

	int id3 = sp.addPolygon({pt2prtl(coords1), pt2prtl(portalContour)});
	int id4 = sp.addPolygon({pt2prtl(coords1), pt2prtl(portalContour2)});
	int id1 = sp.addPolygon({pt2prtl(coords2), pt2prtl(portalContour)});
	int id2 = sp.addPolygon({pt2prtl(coords2), pt2prtl(portalContour2)});

	int pid = sp.addPortal({pt2prtl(portalPoly), pt2prtl(coords1), pt2prtl(coords2)});

	double alpha = 3.788 + 3.2;
	double beta = 0.334 + 0.1;
	double distance = 1.2;

	std::vector<int> temp;
	sp.changePortalPartSpeed(pid, 0, false, prtl::vec3(0, 0, -0.05), temp);
	sp.changePolygonPartSpeed(id1, 0, prtl::vec3(0, 0, -0.05));
	sp.changePolygonPartSpeed(id2, 0, prtl::vec3(0, 0, -0.05));
	//for (int i = 0; i < 69; ++i)
		//sp.computeTime(1.0/3.0);
	for (int i = 0; i < 20; ++i)
		sp.computeTime(1.0/3.0);

	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.5));
	Scatter* sc2 = new Scatter(pt::Color(0, 0, 0.5));
	Scatter* pt1 = new Scatter(pt::Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(pt::Color(0.1, 0.55, 1));
	
	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(-0.75, -0.75, 3), pt::Color(0.5, 0.5, 0.5).sqrt()));
	ren.luminaries.push_back(PointLightSource(vec3(0, 1, 3), pt::Color(0.2, 0.2, 0.2).sqrt()));

	addFloor(scene, 5, -0.5, pt::Color(0.2, 0.2, 0.2));

	for (int i = 0; i < sp.getPolygonCount(); i++) {
		int id = sp.getPolygonId(i);
		for (int j = 0; j < sp.getPolygonPartsCount(id); j++) {
			auto poly = sp.getPolygonPart(id, j);
			auto coords = sp.getPolygonPartCoordSystem(id, j);
			if (id == id3 || id == id4)
				scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords), pt1));
			else
				scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords), pt2));
		}
	}

	for (int i = 0; i < sp.getPortalCount(); i++) {
		int id = sp.getPortalId(i);
		for (int j = 0; j < sp.getPortalPartsCount(id); j++) {
			auto poly = sp.getPortalPart(id, j);
			auto coords1 = sp.getPortalPartCoordSystem(id, j, true);
			auto coords2 = sp.getPortalPartCoordSystem(id, j, false);
			Portals* prr = new Portals(prtl2pt(coords2), prtl2pt(coords1), prtl2pt(poly), pt2, pt1);
			scene.array.push_back(prr);
			//scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords2), sc2));
			//scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords1), sc2));
		}
	}

	//scene.array.push_back(new CubeMap("cubemap.bmp"));
	scene.array.push_back(new MathRoom());
}*/

void PortalWindow::createScene() {
	using namespace pt;
	using namespace prtl;

	MaterialMap polyMm;
	MaterialMap prtlMm;
	PortalSpace sp;

	addRoom(sp, polyMm, vec3(0), 6);

	double angle = twg::deg2rad(20);
	double l = 1.0 / cos(angle);

	std::vector<vec2> pPortal;
	pPortal.push_back({ 0, 0 });
	pPortal.push_back({ 0, 1 });
	pPortal.push_back({ l, 1 });
	pPortal.push_back({ l, 0 });

	CoordSystem cPortal11 = standard;
	cPortal11.pos = vec3(1, -1, 0);
	std::swap(cPortal11.j, cPortal11.k);
	cPortal11.k.negate();
	cPortal11 = rotateAroundAxis(cPortal11, cPortal11.pos, vec3(0, 0, 1), -angle);

	CoordSystem cPortal21 = standard;
	cPortal21.pos = vec3(-0.5, -0.5, 0);
	cPortal21.k.negate();
	cPortal21.i.negate();
	cPortal21 = rotateAroundAxis(cPortal21, cPortal21.pos, vec3(0, 1, 0), -angle);

	auto contourArr2 = addContour(sp, polyMm, pPortal, cPortal21, cPortal21, 0.03, portalSecondColor1, portalFirstColor1);

	int pid = sp.addPortal({ pPortal, cPortal11, cPortal21 });
	prtlMm[pid] = { MATERIAL_SCATTER, portalFirstColor1, portalSecondColor1 };

	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.5));
	Scatter* sc2 = new Scatter(pt::Color(0, 0, 0.5));
	Scatter* pt1 = new Scatter(pt::Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(pt::Color(0.1, 0.55, 1));

	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(-0.75, -0.75, 3), pt::Color(0.5, 0.5, 0.5).sqrt()));
	ren.luminaries.push_back(PointLightSource(vec3(0, 1, 3-0.1), pt::Color(0.2, 0.2, 0.2).sqrt()));

	addFloor(scene, 5, -0.5, pt::Color(0.2, 0.2, 0.2));

	for (int i = 0; i < sp.getPolygonCount(); i++) {
		int id = sp.getPolygonId(i);
		for (int j = 0; j < sp.getPolygonPartsCount(id); j++) {
			auto poly = sp.getPolygonPart(id, j);
			auto coords = sp.getPolygonPartCoordSystem(id, j);
			scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords), pt1));
		}
	}

	for (int i = 0; i < sp.getPortalCount(); i++) {
		int id = sp.getPortalId(i);
		for (int j = 0; j < sp.getPortalPartsCount(id); j++) {
			auto poly = sp.getPortalPart(id, j);
			auto coords1 = sp.getPortalPartCoordSystem(id, j, true);
			auto coords2 = sp.getPortalPartCoordSystem(id, j, false);
			Portals* prr = new Portals(prtl2pt(coords2), prtl2pt(coords1), prtl2pt(poly), pt2, pt1);
			scene.array.push_back(prr);
			//scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords2), sc2));
			//scene.array.push_back(new pt::Polygon(prtl2pt(poly), prtl2pt(coords1), sc2));
		}
	}

	scene.array.push_back(new CubeMap("cubemap.bmp"));
	//scene.array.push_back(new MathRoom());
}

void PortalWindow::func() {
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(ICON_APPLICATION,
		Point_i(100, 0),
		Point_i(300, 300), 
		Point_i(100, 100),
		stdMax,
		L"Computing portal in portal...",
		WINDOW_STANDART);	
	PortalWindow wnd(type);
	wnd.waitForClose();
}