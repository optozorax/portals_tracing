/** Рисуется вращающийся куб при помощи рейтресинга и одного источника света. Далее эта картинка переводится в картинку twg и рисуется на окне WINDOW_IMAGE32. При этом окно закрывается на esc, его можно двигать мышкой. */

// #include <twg/twg.h>
// #include <pt/pt.h>
// #include <pt/pt2twg.h>

// int main() {
// 	using namespace twg;
// 	using namespace pt;

// 	WindowType type(stdIcon,
// 		Point_i(100, 100),
// 		Point_i(300, 300),
// 		stdMin, stdMax,
// 		L"Rotated sphere",
// 		WINDOW_IMAGE32);
// 	WindowBase wnd(type);

// 	ImageBase img1(500, 250);
// 	Image img2(500, 250);
// 	PerspectiveCamera cam(1, pi/2, Vector(3, 1, 3));
// 	cam.lookAt(Vector(0));

// 	RayRenderer ren;
// 	ren.skyColor = Vector(0.5, 0.5, 0.5);
// 	ren.luminaries.push_back(PointLightSource(Vector(3, 3, 3), Color(1, 1, 1)));

// 	Sphere sphere(Vector(0), 1, new Scatter(Color(0.9, 0, 0.9)));
// 	int32 i = 0;
// 	while (!wnd.isClosed()) {
// 		i++;
// 		ren.render(cam, img2, sphere);
// 		toTwgImage(img2, img1);
// 		wnd.updateImage(img1);
// 	}
// }


#include <windows.h>

#include <twg/twg.h>
#include <twg/window/window_events.h>
#include <twg/image/image_drawing.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/shape/sphere.h>
#include <pt/shape/triangle.h>
#include <pt/material/scatter.h>
#include <pt/renderer/ray-tracing.h>
#include <pt/pt2twg.h>

using namespace twg;

class MyWindow : public WindowEvents
{
public:
	MyWindow(WindowType type);

	pt::Image				img;
	pt::PerspectiveCamera 	cam;
	pt::RayRenderer 		ren;
	pt::Scene				scene;
	ImageBase				img2;
	double					alpha;
	double					beta;
	double					distance;
	bool					moveRegime;
	Point_i					last;

	void redraw(void);

	//-------------------------------------------------------------------------
	bool onMouse(Point_i pos, MouseType type);
	bool onResize(Rect rect, SizingType type);
	bool onFocus(bool isKilled);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
MyWindow::MyWindow(WindowType type) : 
	WindowEvents(type),
	img(type.size.x, type.size.y),
	img2(type.size),
	cam(1, pi/2.0, 0, pt::Vector(0), type.size.x, type.size.y),
	moveRegime(false) {
	using namespace pt;
	ren.skyColor = pt::Color(1, 1, 1);
	ren.luminaries.push_back(PointLightSource(Vector(0, 0, 3), pt::Color(1.2, 1.2, 1.2)));
	ren.luminaries.push_back(PointLightSource(Vector(0, 1, 3), pt::Color(1.2, 1.2, 1.2)));

	beta = pt::pi/12;
	alpha = pt::pi;
	distance = 3;

	const double size = 5;
	const double depth = -3;
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
}

//-----------------------------------------------------------------------------
void MyWindow::redraw(void) {
	using namespace pt;
	Vector pos(sin(pt::pi/2 - beta) * cos(alpha), sin(pt::pi/2 - beta) * sin(alpha), cos(pt::pi/2 - beta));
	pos *= distance;
	cam.assign(1, pt::pi/2.0, 0, pos, img.getWidth(), img.getHeight());
	cam.lookAt(Vector(0, 0, 0));

	ren.render(cam, img, scene);
	img.colorCorrection();
	
	toTwgImage(img, img2);
	img2.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img2.size());
}

//-----------------------------------------------------------------------------
bool MyWindow::onMouse(Point_i pos, MouseType type) {
	switch (type) {
		case MOUSE_L_DOWN:
			moveRegime = true;
			last = pos;
			break;
		case MOUSE_L_UP:
			moveRegime = false;
			break;
		case MOUSE_WHEEL_DOWN:
			if (distance < 10) {
				distance += 0.1;
				redraw();
			}
			break;
		case MOUSE_WHEEL_UP:
			if (distance > 1) {
				distance -= 0.1;
				redraw();
			}
			break;
		case MOUSE_MOVE:
			if (moveRegime) {
				Point_i diff = last - pos;
				alpha += diff.x * 0.008;
				beta += -diff.y * 0.008;
				if (beta < -pt::pi/2)
					beta = -pt::pi/2;
				if (beta > pt::pi/2)
					beta = pt::pi/2;
				redraw();
				last = pos;
			}
			break;
	}
	return true;
}

//-----------------------------------------------------------------------------
bool MyWindow::onResize(Rect rect, SizingType type) {
	img.resize(rect.x(), rect.y());
	img2.resize(Point_i(rect.x(), rect.y()));
	redraw();
	return true;
}

//-----------------------------------------------------------------------------
bool MyWindow::onFocus(bool isKilled) {
	if (isKilled)
		moveRegime = false;
	return true;
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
		L"Test events of WindowEvents class.",
		WINDOW_STANDART);	
	MyWindow wnd(type);
	wnd.redraw();
	wnd.waitForClose();
}