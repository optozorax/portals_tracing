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
using namespace pt;
using namespace prtl;

//-----------------------------------------------------------------------------
class PortalWindow : public PtWindow
{
public:
	PortalWindow(WindowType type);

	void setFirstCam(double& focal, double& viewAngle, vec3& pos, vec3& lookAt);
	void createScene(Scene& scene);
	void onRightMouseClick(void);

	PortalSpace sp;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
PortalWindow::PortalWindow(WindowType type) : cam(1, pi/2, 0, vec3(0), 1, 1), ren(cam, scene, img, 1, false), PtWindow(type, &ren, &cam, &scene) {}

//-----------------------------------------------------------------------------
void PortalWindow::onChangeCam(double alpha, double beta, double distance, int width, int height) {
	using namespace pt;
	pos = vec3(sin(pi/2 - beta) * cos(alpha), sin(pi/2 - beta) * sin(alpha), cos(pi/2 - beta));
	pos *= distance;
	vec3 lookAt(0);
	cam.assign(1, pi/2.0, 0, pos, width, height);
	cam.lookAt(lookAt);
}

void addCoordSys(Scene& scene, CoordSystem c) {
	Scatter* sc1 = new Scatter(Color(0.3, 0.3, 0.7));
	Scatter* sc2 = new Scatter(Color(0.3, 0.7, 0.3));
	Scatter* sc3 = new Scatter(Color(0.7, 0.3, 0.3));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.i, 0.02, sc1));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.j, 0.02, sc2));
	scene.array.push_back(new Cylinder(c.pos, c.pos + c.k, 0.02, sc3));
}

void PortalWindow::createScene() {
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

	Scatter* sc = new Scatter(Color(0.5, 0.5, 0.5));
	Scatter* sc2 = new Scatter(Color(0, 0, 0.5));
	Scatter* pt1 = new Scatter(Color(1, 0.5, 0.15));
	Scatter* pt2 = new Scatter(Color(0.1, 0.55, 1));

	ren.skyColor = Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(vec3(-0.75, -0.75, 3), Color(0.5, 0.5, 0.5).sqrt()));
	ren.luminaries.push_back(PointLightSource(vec3(0, 1, 3-0.1), Color(0.2, 0.2, 0.2).sqrt()));

	for (int i = 0; i < sp.getPolygonCount(); i++) {
		int id = sp.getPolygonId(i);
		for (int j = 0; j < sp.getPolygonPartsCount(id); j++) {
			auto poly = sp.getPolygonPart(id, j);
			auto coords = sp.getPolygonPartCoordSystem(id, j);
			scene.array.push_back(new Polygon(poly, coords, pt1));
		}
	}

	for (int i = 0; i < sp.getPortalCount(); i++) {
		int id = sp.getPortalId(i);
		for (int j = 0; j < sp.getPortalPartsCount(id); j++) {
			auto poly = sp.getPortalPart(id, j);
			auto coords1 = sp.getPortalPartCoordSystem(id, j, true);
			auto coords2 = sp.getPortalPartCoordSystem(id, j, false);
			Portals* prr = new Portals(coords2, coords1, poly, pt2, pt1);
			scene.array.push_back(prr);
			//scene.array.push_back(new Polygon(prtl2pt(poly), prtl2pt(coords2), sc2));
			//scene.array.push_back(new Polygon(prtl2pt(poly), prtl2pt(coords1), sc2));
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