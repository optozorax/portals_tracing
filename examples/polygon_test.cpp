#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/triangle.h>
#include <pt/shape/polygon.h>
#include <pt/shape/sphere.h>
#include <pt/material/scatter.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

using namespace pt;

void initScene(Scene& scene) {
	auto& array = scene.array;
	Scatter* sc = new Scatter(Color(0.5, 0.5, 0.7));

	{
		const double size = 10;
		const double depth = -1.5;
		Vector a(-size, -size, depth);
		Vector b(-size, size, depth);
		Vector c(size, size, depth);
		Vector d(size, -size, depth);
		scene.array.push_back(new Triangle(a, b, c, new Scatter(pt::Color(0.1, 0, 0.1))));
		scene.array.push_back(new Triangle(c, d, a, new Scatter(pt::Color(0.1, 0, 0.1))));
	}

	Coords coords1;
	coords1.pos = Vector(0);
	coords1.i = Vector(1, 0, 0);
	coords1.j = Vector(0, 0, 1);
	coords1.k = Vector(0, 1, 0);

	{
		std::vector<Point> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 0});
		array.push_back(new Polygon(mas, coords1, sc));

		Coords coords2;
		coords2.pos = Vector(0, 2, -0.5);
		coords2.i = Vector(1.0/sqrt(2.0), 0, -1.0/sqrt(2.0));
		coords2.j = Vector(1.0/sqrt(2.0), 0, 1.0/sqrt(2.0));
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(new Polygon(mas, coords2, sc));

		coords2.pos = Vector(1.5, 2, -0.5);
		coords2.i = Vector(1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
		coords2.j = Vector(1.0/sqrt(2.0), 0, 1.0/sqrt(2.0));
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(new Polygon(mas, coords2, sc));

		coords2.pos = Vector(-1.5, 2, -0.5);
		coords2.i = Vector(1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
		coords2.j = Vector(1.0/sqrt(2.0), 1.0/sqrt(2.0), 0);
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(new Polygon(mas, coords2, sc));
	}

	{
		Coords coords2 = coords1;
		coords2.pos = Vector(1.5, 0, 0);
		std::vector<Point> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 1});
		mas.push_back({1, 0});
		array.push_back(new Polygon(mas, coords2, sc));
	}

	{
		Coords coords2 = coords1;
		coords2.pos = Vector(-1.5, 0, 0);
		std::vector<Point> mas;
		for (int i = 0; i < 359; i++)
			mas.push_back({0.5 + 0.5 * sin(i/180.0 * pi), 0.5 + 0.5 * cos(i/180.0 * pi)});
		array.push_back(new Polygon(mas, coords2, sc));
	}

	{
		Coords coords2 = coords1;
		coords2.pos = Vector(-3, 0, 0);
		std::vector<Point> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 0});
		mas.push_back({1, 1});
		array.push_back(new Polygon(mas, coords2, sc));
	}

	{
		Coords coords2 = coords1;
		coords2.pos = Vector(3, 0, 0);
		std::vector<Point> mas;
		mas.push_back({31/290.0, 34/313.0});
		mas.push_back({290/290.0, 100/313.0});
		mas.push_back({0/290.0, 199/313.0});
		mas.push_back({165/290.0, 313/313.0});
		mas.push_back({271/290.0, 226/313.0});
		mas.push_back({177/290.0, 0/313.0});
		mas.push_back({167/290.0, 240/313.0});
		mas.push_back({55/290.0, 108/313.0});
		mas.push_back({194/290.0, 113/313.0});
		array.push_back(new Polygon(mas, coords2, sc));
	}
}

int main() {
	Scene scene;
	initScene(scene);

	Image img(500, 250);

	PerspectiveCamera cam(2, 45*pt::pi/180, 0, Vector(0, 7, 1), img.getWidth(), img.getHeight());
	cam.lookAt(Vector(0, 0, 0));

	RayRenderer ren2(2);
	ren2.luminaries.push_back(PointLightSource(Vector(0, 0, 5), pt::Color(0.5, 0.5, 0.5)));
	ren2.luminaries.push_back(PointLightSource(Vector(5, 5, 3), pt::Color(0.5, 0.5, 0.5)));
	ren2.luminaries.push_back(PointLightSource(Vector(-5, -3, 3), pt::Color(0.5, 0.5, 0.5)));
	ren2.skyColor = pt::Color(0.5, 0.5, 0.5);
	ren2.render(cam, img, scene);
	img.colorCorrection();
	saveAsBmp(img, "polygon_test.bmp");

	return 0;
}