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
#include <pt/pt2easybmp.h>

int main() {
	using namespace pt;
	bool isDrawHints = false;

	Image img(1000, 500);
	Scene scene;

	// Создаем камеру
	vec3 camPos = {0, -4, 4};
	vec3 lookAt = {0, 0, 0.5};
	PerspectiveCamera cam(1, pi / 2.0, 0, camPos, img.getWidth(), img.getHeight());
	cam.lookAt(lookAt);

	RayTracing ren(2, 4, true);
	ren.setAmbientLight(Color(0.7, 0.7, 0.7, 1));

	// Создаем пол
	const double size = 500;
	std::vector<vec2> mas2;
	mas2.push_back({-size, -size});
	mas2.push_back({-size, size});
	mas2.push_back({size, size});
	mas2.push_back({size, -size});
	crd3 floor = getStandardCrd3();
	scene.add(makePolygon(mas2, floor, makeScatter(Color(0.4, 0, 0.6))));

	Color portalFirstColor0 = Color(1, 0.5, 0.15); // orange
	Color portalSecondColor0 = Color(0.1, 0.55, 1); // blue
	Color portalFirstColor1 = Color(0.67, 0.02, 0.02); // red
	Color portalSecondColor1 = Color(0.33, 0, 1); // dark blue

	// Массивы для порталов
	std::vector<vec2> mas;
	mas.push_back({-1, -1});
	mas.push_back({-1, 1});
	mas.push_back({1, 1});
	mas.push_back({1, -1});

	double h = 0.1;
	std::vector<vec2> contour;
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
	crd3 p11 = getStandardCrd3();
	crd3 p12 = getStandardCrd3();
	p11.pos = {0, 0, 1};
	p12.pos = {2 + 2*h, 2, 1};
	Portals_ptr prt = makePortals(p11, p12, mas, makeScatter(portalFirstColor0), makeScatter(portalSecondColor0));
	scene.add(prt);
	ren.addPortal(prt);
	scene.add(makePolygon(contour, p11, makeScatter(portalFirstColor0)));
	scene.add(makePolygon(contour, p12, makeScatter(portalSecondColor0)));

	// Вторая пара порталов
	crd3 p21 = getStandardCrd3();
	crd3 p22 = getStandardCrd3();
	p21.pos = p12.pos;
	p21.pos.z += 1;
	p22.pos = {0, 2, 2};
	Portals_ptr prt1 = makePortals(p21, p22, mas, makeScatter(portalFirstColor1), makeScatter(portalSecondColor1));
	scene.add(prt1);
	ren.addPortal(prt1);
	scene.add(makePolygon(contour, p21, makeScatter(portalFirstColor1)));
	scene.add(makePolygon(contour, p22, makeScatter(portalSecondColor1)));

	// Источник освещения
	vec3 lightPos = p22.pos;
	lightPos.z += 1;
	lightPos.x += 0.5;
	ren.luminaries.push_back(PointLight(lightPos, Color(0.5, 0.5, 0.5)));

	// Добавляем сферы, которые показывают положение источника освещения
	double spsize = 0.05;
	vec3 spherePos = lightPos + vec3(0, 0, spsize + 0.01);
	scene.add(makeSphere(spherePos, spsize, makeScatter(Color(1, 1, 1, 1))));

	if (isDrawHints) {
		vec3 spherePos1 = space3(prt1->p1).from(space3(prt1->p2).to(spherePos));
		scene.add(makeSphere(spherePos1, spsize, makeScatter(Color(0.5, 0.5, 0.5, 1))));
		vec3 spherePos2 = space3(prt->p1).from(space3(prt->p2).to(spherePos1));
		scene.add(makeSphere(spherePos2, spsize, makeScatter(Color(0.25, 0.25, 0.25, 1))));
	}

	// Добавляем небо
	scene.add(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));

	// Для отладки
	/*Ray ray;
	ray.pos = vec3(0, -4, 1);
	ray.dir = -ray.pos.normalize();
	ren.computeColor(ray);*/

	// Рендерим первую картинку
	ren.assign(&cam, &scene, &img);
	ren.render();
	img.colorCorrection();
	if (isDrawHints)
		saveAsPng(img, "portal_light_test1_hints_1.png");
	else
		saveAsPng(img, "portal_light_test1_1.png");

	// Рендерим вторую картинку с другого угла
	cam.pos = {0, -3, 1.2};
	cam.lookAt({0, 0, 0.5});
	ren.render();
	img.colorCorrection();
	if (isDrawHints)
		saveAsPng(img, "portal_light_test1_hints_2.png");
	else
		saveAsPng(img, "portal_light_test1_2.png");

	// Рендерим картинку сверху
	Orthogonal cam2(vec3(1, 1, 5), 10.0 / img.getWidth() , img.getWidth(), img.getHeight());
	cam2.lookTowards(vec3(1, 1 - 0.001, 0));
	ren.assign(&cam2, &scene, &img);
	ren.render();
	img.colorCorrection();
	if (isDrawHints)
		saveAsPng(img, "portal_light_test1_hints_3.png");
	else
		saveAsPng(img, "portal_light_test1_3.png");
}