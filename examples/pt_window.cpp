#include "pt_window.h"

namespace pt
{

using namespace twg;

//-----------------------------------------------------------------------------
PtWindow::PtWindow(WindowType type) : 
	WindowEvents(type),
	img(type.size.x, type.size.y),
	img2(type.size),
	moveRegime(false) {
}

//-----------------------------------------------------------------------------
void PtWindow::start(void) {
	createScene();

	beta = pt::pi/6;
	alpha = 5*pt::pi/6;
	distance = 3;
	onChangeCam(alpha, beta, distance);

	redraw();
}

//-----------------------------------------------------------------------------
void PtWindow::redraw(void) {
	using namespace pt;
	using namespace twg;
	onDraw(img);
	
	toTwgImage(img, img2);
	img2.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img2.size());
}

//-----------------------------------------------------------------------------
bool PtWindow::onMouse(Point_i pos, MouseType type) {
	const double distanceStep = 0.2;
	const double maxDistance = 100;
	const double moveCoeff = 0.008;

	switch (type) {
		case MOUSE_L_DOWN:
			moveRegime = true;
			last = pos;
			break;
		case MOUSE_L_UP:
			moveRegime = false;
			break;
		case MOUSE_WHEEL_DOWN:
			if (distance < maxDistance) {
				distance += distanceStep;
				onChangeCam(alpha, beta, distance);
				redraw();
			}
			break;
		case MOUSE_WHEEL_UP:
			if (distance > distanceStep) {
				distance -= distanceStep;
				onChangeCam(alpha, beta, distance);
				redraw();
			}
			break;
		case MOUSE_MOVE:
			if (moveRegime) {
				Point_i diff = last - pos;
				alpha += diff.x * moveCoeff;
				beta += -diff.y * moveCoeff;
				if (beta < -pt::pi/2) beta = -pt::pi/2;
				if (beta > pt::pi/2)  beta = pt::pi/2;
				onChangeCam(alpha, beta, distance);
				redraw();
				last = pos;
			}
			break;
	}
	return true;
}

//-----------------------------------------------------------------------------
bool PtWindow::onResize(Rect rect, SizingType type) {
	img.resize(rect.x(), rect.y());
	img2.resize(Point_i(rect.x(), rect.y()));
	redraw();
	return true;
}

//-----------------------------------------------------------------------------
bool PtWindow::onFocus(bool isKilled) {
	if (isKilled) 
		moveRegime = false;
	return true;
}

}