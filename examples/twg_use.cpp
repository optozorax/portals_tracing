#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/material/scatter.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2twg.h>

#include "portals.h"
#include "pt_window.h"

using namespace twg;

//-----------------------------------------------------------------------------
class PortalWindow : public pt::PtWindow
{
public:
	PortalWindow(WindowType type);

	void onChangeCam(double alpha, double beta, double distance);
	void onDraw(pt::Image& img);
	void createScene(void);	

	pt::PerspectiveCamera	cam;
	pt::Scene				scene;
	pt::RayRenderer			ren;
	pt::Vector				pos;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
PortalWindow::PortalWindow(WindowType type) : PtWindow(type), cam(1, pi/2, 0, pt::Vector(0), 1, 1) {}

//-----------------------------------------------------------------------------
void PortalWindow::onChangeCam(double alpha, double beta, double distance) {
	using namespace pt;
	pos = Vector(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta));
	pos *= distance;
}

//-----------------------------------------------------------------------------
void PortalWindow::onDraw(pt::Image& img) {
	using namespace pt;
	cam.assign(1, pt::pi/2.0, 0, pos, img.getWidth(), img.getHeight());
	cam.lookAt(Vector(0, 0, 0));

	ren.render(cam, img, scene);
	img.colorCorrection();
}

//-----------------------------------------------------------------------------
void PortalWindow::createScene(void) {
	using namespace pt;
	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(Vector(0, 0, 3), pt::Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(Vector(0, 1, 3), pt::Color(1.2, 1.2, 1.2)));

	const double size = 5;
	const double depth = -1.15;
	Vector a(-size, -size, depth);
	Vector b(-size, size, depth);
	Vector c(size, size, depth);
	Vector d(size, -size, depth);
	scene.array.push_back(new Sphere(new Scatter(pt::Color(0.1, 0.1, 0.1))));
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
	scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));

	a = Vector(0, 0, 1);
	b = Vector(0.5, 1.2, 1);
	c = Vector(1.2, 1.2, 1.2);
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0.1, 0.2))));

	//-------------------------------------------------------------------------
	// Треугольные порталы
	// const double psize = 4;
	// const double pheight = 4;
	// a = Vector(psize, psize, -1);
	// b = Vector(psize, -psize, -1);
	// c = Vector(psize, 0, -1 + pheight);
	// Vector a1(-psize, psize, -1);
	// Vector b1(-psize, -psize, -1);
	// Vector c1(-psize, 0, -1 + pheight);
	// scene.array.push_back(new Portals(a, b, c, a1, b1, c1, pt::Color(0.5, 0.5, 0.5)));

	// const double thick = 0.15;
	// Vector diff(0.01, 0, 0);
	// Vector diffa(0, 2*thick, -thick);
	// Vector diffb(0, -2*thick, -thick);
	// Vector diffc(0, 0, thick);
	// a += diff + diffa;
	// b += diff + diffb;
	// c += diff + diffc;
	// scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.2, 0.2, 0.6))));
	// a1 -= diff - diffa;
	// b1 -= diff - diffb;
	// c1 -= diff - diffc;
	// scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0.6, 0.3, 0.2))));

	//-------------------------------------------------------------------------
	// Квадратные порталы
	const double psize = 4;
	const double pheight = 5;
	a = Vector(psize, psize, -1);
	b = Vector(psize, -psize, -1);
	c = Vector(psize, -psize, -1 + pheight);
	d = Vector(psize, psize, -1 + pheight);
	Vector a1(-psize, psize, -1);
	Vector b1(-psize, -psize, -1);
	Vector c1(-psize, -psize, -1 + pheight);
	Vector d1(-psize, psize, -1 + pheight);
	scene.array.push_back(new Portals(a, b, c, a1, b1, c1, pt::Color(0.5, 0.5, 0.5)));
	scene.array.push_back(new Portals(a, c, d, a1, c1, d1, pt::Color(0.5, 0.5, 0.5)));

	const double thick = 0.15;
	Vector diff(0.01, 0, 0);
	Vector diffa(0, thick, -thick);
	Vector diffb(0, -thick, -thick);
	Vector diffc(0, -thick, thick);
	Vector diffd(0, thick, thick);
	a += diff + diffa;
	b += diff + diffb;
	c += diff + diffc;
	d += diff + diffd;
	scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.2, 0.2, 0.6))));
	scene.array.push_back(new Triangle(a, c, d, new Scatter(pt::Color(0.2, 0.2, 0.6))));
	a1 -= diff - diffa;
	b1 -= diff - diffb;
	c1 -= diff - diffc;
	d1 -= diff - diffd;
	scene.array.push_back(new Triangle(a1, b1, c1, new Scatter(pt::Color(0.6, 0.3, 0.2))));
	scene.array.push_back(new Triangle(a1, c1, d1, new Scatter(pt::Color(0.6, 0.3, 0.2))));
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
		L"Sqare portals.",
		WINDOW_STANDART);	
	PortalWindow wnd(type);
	wnd.start();
	wnd.waitForClose();
}