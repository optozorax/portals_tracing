#include <iostream>
#include <sstream>
#include <iomanip>
#include <windows.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/object/texture_polygon.h>
#include <pt/shape/sphere.h>
#include <pt/shape/polygon.h>
#include <pt/shape/portals.h>
#include <pt/material/scatter.h>
#include <pt/material/reflect.h>
#include <pt/camera/360.h>
#include <pt/camera/orthogonal.h>
#include <pt/renderer1.h>
#include <pt/pt2easybmp.h>
#include <pt/pt2twg.h>

#include <twg/twg.h>
#include <twg/image/image_drawing.h>

int main() {
	using namespace pt;
	using namespace prtl;

	Image img(1000, 500);
	Scene scene;

	// Создаем камеру
	double angle = 0;
	vec3 camPos = { 5*cos(angle), 5 * sin(angle), 5 };
	vec3 lookAt = { 0, 0, 0.5 };
	PerspectiveCamera cam(1, pi / 2.0, 0, camPos, img.getWidth(), img.getHeight());
	cam.lookAt(lookAt);

	RayTracing ren(3, 4, true);
	ren.assign(&cam, &scene, &img);

	// Создаем пол
	const double size = 500;
	std::vector<vec2> mas2;
	mas2.push_back({ -size, -size });
	mas2.push_back({ -size, size });
	mas2.push_back({ size, size });
	mas2.push_back({ size, -size });
	crd3 floor = getStandardCrd3();
	scene.array.push_back(makePolygon(mas2, floor, makeScatter(Color(0.4, 0, 0.6))));

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
	twg::ImageDrawing_aa img2({300, 300});
	img2.clear(twg::Transparent);
	img2.setPen(twg::Pen(10, twg::Black));
	img2.drawLine({10, 10}, {200, 200});
	img2.setTextStyle(twg::TextStyle(50, L"Consolas", twg::TEXT_NONE, false));
	img2.drawText({100, 100}, L"Hello world");
	img2.setTextStyle(twg::TextStyle(50, L"Consolas", twg::TEXT_NONE, true));
	img2.drawText({ 100, 200 }, L"Hello world");
	Image img3(1, 1);
	toPtImage(img3, img2);

	crd3 p11 = getStandardCrd3();
	std::swap(p11.j, p11.k);
	crd3 p12 = p11;
	p11.pos = { 0, 2, 3 };
	p12.pos = { 0, -2, 3 };
	FindBorders brd(300, 0);
	brd.process(mas);
	brd.finish();
	scene.array.push_back(makeTexturePolygon(mas, p11, &img3, brd));
	scene.array.push_back(makeTexturePolygon(mas, p12, &img3, brd));
	scene.array.push_back(makePolygon(contour, p11, makeScatter(portalFirstColor0)));
	scene.array.push_back(makePolygon(contour, p12, makeScatter(portalSecondColor0)));

	// Источник освещения
	vec3 lightPos = p12.pos;
	lightPos.y = 0;
	lightPos.z += 3.5;
	lightPos.z += 0.5;
	ren.luminaries.push_back(PointLight(lightPos, Color(0.5, 0.5, 0.5)));

	// Добавляем сферы, которые показывают положение источника освещения
	double spsize = 0.05;
	vec3 spherePos = lightPos + vec3(0, 0, spsize + 0.01);
	scene.array.push_back(makeSphere(spherePos, spsize, makeScatter(Color(1, 1, 1))));

	// Добавляем небо
	scene.array.push_back(makeSky(Color(0.3, 0.3, 0.9), Color(1, 1, 1)));

	// Рендерим
	ren.render();
	img.colorCorrection();
	saveAsBmp(img, "texture_polygon_test.bmp");
}