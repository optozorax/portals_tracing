#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/triangle.h>
#include <pt/shape/polygon.h>
#include <pt/shape/sphere.h>
#include <pt/shape/contour.h>
#include <pt/shape/portals.h>
#include <pt/shape/cylinder.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

using namespace pt;

void addContour(Scene& scene, const plane3& c, std::vector<vec2> poly, Material_ptr material) {
	double thick = 0.01;
	poly.push_back(poly[0]);
	poly.push_back(poly[poly.size() - 2]);
	std::vector<vec3> mas;
	for (int i = 1; i < poly.size() - 1; i++) {
		vec3 a = c.from(poly[i - 1]) - c.from(poly[i]);
		vec3 b = c.from(poly[i + 1]) - c.from(poly[i]);
		vec3 dir = -(a + b);
		dir.normalize();
		mas.push_back(c.from(poly[i]) + dir * thick);
	}
	mas.push_back(mas.front());

	scene.add(makeContour(mas, thick, false, material));
}

void initScene(Scene& scene, RayTracing& ren) {
	Material_ptr sc = makeScatter(pt::Color(0.5, 0.5, 0.7, 0.5));

	Material_ptr pt1 = makeScatter(pt::Color(1, 0.5, 0.15));
	Material_ptr pt2 = makeScatter(pt::Color(0.1, 0.55, 1));

	{
		const double size = 50;
		const double depth = -0.5;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.add(makeTriangle(a, b, c, makeScatter(pt::Color(0.35, 0, 0.35))));
		scene.add(makeTriangle(c, d, a, makeScatter(pt::Color(0.35, 0, 0.35))));
	}

	{
		crd3 coords1;
		coords1.i = vec3(1, 0, 0);
		coords1.j = vec3(0, 0, 1);
		coords1.k = vec3(0, 1, 0);
		coords1.pos = vec3(-0.5, 0.5, 2);
		coords1 = rotate(coords1, vec3(0, -pt::pi/2, 0));

		std::vector<vec2> mas2;
		mas2.push_back({0, 0});
		mas2.push_back({0, 1});
		mas2.push_back({1, 1});
		mas2.push_back({1, 0});

		crd3 coords2;
		coords2 = coords1;
		coords2.pos = vec3(-0.5, 0.5, 1.8);

		Portals_ptr prt = makePortals(coords2, coords1, mas2, pt1, pt2);
		scene.add(prt);
		ren.addPortal(prt);
		addContour(scene, coords2, mas2, pt1);
		addContour(scene, coords1, mas2, pt2);

		coords1.pos = vec3(-0.5, 0.5, 1.4);
		coords2.pos = vec3(-0.5, 0.5, 1);

		Portals_ptr prt1 = makePortals(coords2, coords1, mas2, pt1, pt2);
		scene.add(prt1);
		ren.addPortal(prt1);
		addContour(scene, coords2, mas2, pt1);
		addContour(scene, coords1, mas2, pt2);

		coords2.pos = vec3(-0.5, 0.5, 0.5);
		scene.add(makePolygon(mas2, coords2, sc));

		coords1.pos = vec3(-0.5, 0.5, 3);
		scene.add(makePolygon(mas2, coords1, sc));

		coords1.pos = vec3(-0.5, 0.5, 1.6);
		scene.add(makePolygon(mas2, coords1, sc));
	}
}

int main() {
	Scene scene;

	Image img(500, 500);

	PerspectiveCamera cam(2, 45*pt::pi/180, 0, vec3(2, 3, 1), img.getWidth(), img.getHeight());
	cam.lookAt(vec3(2, 0, 0.5));

	double beta = 0.814;
	double alpha = -0.612;
	double distance = 2.7;
	vec3 pos = vec3(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta));
	pos *= distance;
	pos = pos + vec3(0, 0, 0.5);
	cam.assign(1, pt::pi/2.0, 0, pos, img.getWidth(), img.getHeight());
	cam.lookAt(vec3(0, 0, 0.5));

	RayTracing ren(2, 4, true);
	ren.luminaries.push_back(PointLight(vec3(0, 0, 4), pt::Color(1, 1, 1)));
	scene.add(makeSky(Color(1, 1, 1), Color(1, 1, 1)));

	initScene(scene, ren);

	ren.assign(&cam, &scene, &img);
	ren.render();
	img.colorCorrection();
	saveAsBmp(img, "portal_light.bmp");
}