#include <iostream>
#include <windows.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/sphere.h>
#include <pt/shape/polygon.h>
#include <pt/shape/portals.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer1.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;

	Image img(1000, 500);
	Scene scene;

	// Создаем камеру
	Vector camPos = {0, -4, 4};
	Vector lookAt = {0, 0, 0.5};
	PerspectiveCamera cam(1, pi / 2.0, 0, camPos, img.getWidth(), img.getHeight());
	cam.lookAt(lookAt);

	RayTracing ren(cam, scene, img, 2);

	// Создаем пол
	const double size = 500;
	std::vector<Vector2> mas2;
	mas2.push_back({-size, -size});
	mas2.push_back({-size, size});
	mas2.push_back({size, size});
	mas2.push_back({size, -size});
	CoordSystem floor = standard;
	scene.array.push_back(new pt::Polygon(mas2, floor, new Scatter(Color(0.4, 0, 0.6))));

	Color portalFirstColor0 = Color(1, 0.5, 0.15); // orange
	Color portalSecondColor0 = Color(0.1, 0.55, 1); // blue
	Color portalFirstColor1 = Color(0.67, 0.02, 0.02); // red
	Color portalSecondColor1 = Color(0.33, 0, 1); // dark blue

	// Массивы для порталов
	std::vector<Vector2> mas;
	mas.push_back({-1, -1});
	mas.push_back({-1, 1});
	mas.push_back({1, 1});
	mas.push_back({1, -1});

	double h = 0.1;
	std::vector<Vector2> contour;
	contour.push_back({-1, 1});
	contour.push_back({-1-h, 1+h});
	contour.push_back({1+h, 1+h});
	contour.push_back({1+h, -1-h});
	contour.push_back({-1-h, -1-h});
	contour.push_back({-1-h, 1+h});
	contour.push_back({-1, 1});
	contour.push_back({1, 1});
	contour.push_back({1, -1});
	contour.push_back({-1, -1});

	// Первая пара порталов
	CoordSystem p11 = standard;
	CoordSystem p12 = standard;
	p11.pos = {0, 0, 1};
	p12.pos = {2 + 2*h, 2, 1};
	scene.array.push_back(new Portals(p11, p12, mas, new Scatter(portalFirstColor0), new Scatter(portalSecondColor0)));
	ren.portals.push_back((Portals*)scene.array.back());
	scene.array.push_back(new pt::Polygon(contour, p11, new Scatter(portalFirstColor0)));
	scene.array.push_back(new pt::Polygon(contour, p12, new Scatter(portalSecondColor0)));

	// Вторая пара порталов
	CoordSystem p21 = standard;
	CoordSystem p22 = standard;
	p21.pos = p12.pos;
	p21.pos.z += 1;
	p22.pos = {0, 2, 2};
	scene.array.push_back(new Portals(p21, p22, mas, new Scatter(portalFirstColor1), new Scatter(portalSecondColor1)));
	ren.portals.push_back((Portals*)scene.array.back());
	scene.array.push_back(new pt::Polygon(contour, p21, new Scatter(portalFirstColor1)));
	scene.array.push_back(new pt::Polygon(contour, p22, new Scatter(portalSecondColor1)));

	// Источник освещения
	Vector lightPos = p22.pos;
	lightPos.z += 1;
	lightPos.x += 0.5;
	ren.luminaries.push_back(PointLight(lightPos, Color(0.5, 0.5, 0.5)));

	// Добавляем сферы, которые показывают положение источника освещения
	double spsize = 0.05;
	Vector spherePos = lightPos + Vector(0, 0, spsize + 0.01);
	scene.array.push_back(new Sphere(spherePos, spsize, new Scatter(Color(1, 1, 1, 1))));
	Vector spherePos1 = teleportVector(ren.portals[1]->p2, ren.portals[1]->p1, spherePos);
	scene.array.push_back(new Sphere(spherePos1, spsize, new Scatter(Color(0.5, 0.5, 0.5, 1))));
	Vector spherePos2 = teleportVector(ren.portals[0]->p2, ren.portals[0]->p1, spherePos1);
	scene.array.push_back(new Sphere(spherePos2, spsize, new Scatter(Color(0.25, 0.25, 0.25, 1))));

	// Добавляем небо
	scene.array.push_back(new Sky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));

	// Для отладки
	/*Ray ray;
	ray.pos = Vector(0, -4, 1);
	ray.dir = -ray.pos.normalize();
	ren.computeColor(ray);*/

	// Рендерим первую картинку
	ren.render();
	img.colorCorrection();
	saveAsBmp(img, "portal_light_test1.bmp");

	// Рендерим вторую картинку с другого угла
	cam.pos = {0, -3, 1.2};
	cam.lookAt({0, 0, 0.5});
	ren.render();
	img.colorCorrection();
	saveAsBmp(img, "portal_light_test2.bmp");
}