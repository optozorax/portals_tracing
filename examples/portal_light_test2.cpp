#include <iostream>

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
#include <pt/renderer.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;
	bool isDrawHints = false;

	Image img(1000, 500);
	Scene scene;

	// Создаем камеру
	vec3 camPos = { 6, 0, 6 };
	vec3 lookAt = { 0, 0, 0.5 };
	PerspectiveCamera cam(1, pi / 2.0, 0, camPos, img.getWidth(), img.getHeight());
	cam.lookAt(lookAt);

	RayTracing ren(2, 4, true);
	ren.assign(&cam, &scene, &img);

	// Создаем пол
	const double size = 500;
	std::vector<vec2> mas2;
	mas2.push_back({ -size, -size });
	mas2.push_back({ -size, size });
	mas2.push_back({ size, size });
	mas2.push_back({ size, -size });
	CoordSystem floor = standard;
	scene.add(new pt::Polygon(mas2, floor, new Scatter(Color(0.4, 0, 0.6))));

	Color portalFirstColor0 = Color(1, 0.5, 0.15); // orange
	Color portalSecondColor0 = Color(0.1, 0.55, 1); // blue
	Color portalFirstColor1 = Color(0.67, 0.02, 0.02); // red
	Color portalSecondColor1 = Color(0.33, 0, 1); // dark blue

	// Массивы для порталов
	std::vector<vec2> mas;
	mas.push_back({ -1, -1 });
	mas.push_back({ -1, 1 });
	mas.push_back({ 1, 1 });
	mas.push_back({ 1, -1 });

	double h = 0.1;
	std::vector<vec2> contour;
	contour.push_back({ -1, 1 });
	contour.push_back({ -1 - h, 1 + h });
	contour.push_back({ 1 + h, 1 + h });
	contour.push_back({ 1 + h, -1 - h });
	contour.push_back({ -1 - h, -1 - h });
	contour.push_back({ -1 - h, 1 + h });
	contour.push_back({ -1, 1 });
	contour.push_back({ 1, 1 });
	contour.push_back({ 1, -1 });
	contour.push_back({ -1, -1 });

	// Первая пара порталов
	CoordSystem p11 = standard;
	std::swap(p11.j, p11.k);
	CoordSystem p12 = p11;
	p11.pos = { 0, 2, 3 };
	p12.pos = { 0, -2, 3 };
	Portals* pp = new Portals(p11, p12, mas, new Scatter(portalFirstColor0), new Scatter(portalSecondColor0));
	scene.add(pp);
	ren.addPortal(pp);
	scene.add(new pt::Polygon(contour, p11, new Scatter(portalFirstColor0)));
	scene.add(new pt::Polygon(contour, p12, new Scatter(portalSecondColor0)));

	if (isDrawHints) {
		p11.pos.y += 4;
		p12.pos.y -= 4;
		scene.add(new pt::Polygon(contour, p11, new Scatter(portalFirstColor0*Color(0.5, 0.5, 0.5))));
		scene.add(new pt::Polygon(contour, p12, new Scatter(portalSecondColor0*Color(0.5, 0.5, 0.5))));
	}

	// Источник освещения
	vec3 lightPos = p12.pos;
	lightPos.z += 0.5;
	lightPos.y = 0;
	ren.luminaries.push_back(PointLight(lightPos, Color(0.5, 0.5, 0.5)));

	// Добавляем сферы, которые показывают положение источника освещения
	double spsize = 0.05;
	vec3 spherePos = lightPos + vec3(0, 0, spsize + 0.01);
	scene.add(new Sphere(spherePos, spsize, new Scatter(Color(1, 1, 1))));

	if (isDrawHints) {
		Portals portal = *pp;
		Portals inverted = invert(portal);
		vec3 spherePos1 = teleportVector(inverted.p1, inverted.p2, spherePos);
		scene.add(new Sphere(spherePos1, spsize, new Scatter(Color(0.5, 0.5, 0.5))));
		vec3 spherePos2 = teleportVector(portal.p1, portal.p2, spherePos);
		scene.add(new Sphere(spherePos2, spsize, new Scatter(Color(0.5, 0.5, 0.5))));
		vec3 spherePos3 = teleportVector(inverted.p1, inverted.p2, spherePos1);
		scene.add(new Sphere(spherePos3, spsize, new Scatter(Color(0.25, 0.25, 0.25))));
		vec3 spherePos4 = teleportVector(portal.p1, portal.p2, spherePos2);
		scene.add(new Sphere(spherePos4, spsize, new Scatter(Color(0.25, 0.25, 0.25))));
	}

	// Добавляем небо
	scene.add(new Sky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));

	// Для отладки
	/*Ray ray;
	ray.pos = vec3(0, -4, 1);
	ray.dir = -ray.pos.normalize();
	ren.computeColor(ray);*/

	// Рендерим
	ren.render();
	img.colorCorrection();
	if (isDrawHints)
		saveAsBmp(img, "portal_light_test2_hints.bmp");
	else
		saveAsBmp(img, "portal_light_test2.bmp");
}