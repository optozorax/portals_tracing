#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/triangle.h>
#include <pt/shape/polygon.h>
#include <pt/shape/sphere.h>
#include <pt/shape/contour.h>
#include <pt/shape/portals.h>
#include <pt/shape/cylinder.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

using namespace pt;

vec3 transform(CoordSystem c, vec2 b) {
	return c.pos + c.i * b.x + c.j * b.y;
}

void addContour(Scene& scene, CoordSystem c, std::vector<vec2> poly, Material* material) {
	double thick = 0.01;
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

	scene.array.push_back(new Contour(mas, thick, material));
}

void initScene(Scene& scene, RayRenderer& ren) {
	Scatter* sc = new Scatter(pt::Color(0.5, 0.5, 0.7, 0.5));

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

	{
		CoordSystem coords1;
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
}

int main() {
	Scene scene;

	Image img(1000, 1000);

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

	RayRenderer ren(2);
	ren.luminaries.push_back(PointLightSource(vec3(0, 0, 4), pt::Color(1, 1, 1)));
	ren.skyColor = pt::Color(0.5, 0.5, 0.5);

	initScene(scene, ren);

	ren.render(cam, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "portal_light.bmp");

	system("portal_light.bmp");

	return 0;
}