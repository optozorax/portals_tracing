#include <sstream>
#include <iomanip>
#include "pt_window.h"

namespace pt
{

using namespace twg;

//-----------------------------------------------------------------------------
PtWindow::PtWindow(WindowType type, Renderer* ren, Camera* cam, Object* scene) : 
	WindowEvents(type),
	img(type.size.x, type.size.y),
	moveRegime(false),
	ren(ren),
	cam(cam),
	scene(scene),
	isStarted(false) {
}

//-----------------------------------------------------------------------------
PtWindow::~PtWindow() {
	ren.stop();
}

//-----------------------------------------------------------------------------
void PtWindow::start(void) {
	createScene();

	img.resize(getClientSize().x, getClientSize().y);
	ren.start(cam, &img, scene);

	//beta = pt::pi/5;
	//alpha = -pt::pi/3 + 0.3;
	//beta = 0.814;
	//alpha = -0.612;
	//distance = 1.5;
	beta = 0.294;
	alpha = -0.252;
	distance = 3.1;
	changeCam();

	SetTimer(getHwnd(), 1220, 1000/fps, NULL);
}

//-----------------------------------------------------------------------------
void PtWindow::changeCam() {
	long time = GetTickCount();

	isEnter = false;
	ImageBase img1(ren.getCurrentImage(true));
	img1.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img1.size());
	ren.stop();
	onChangeCam(alpha, beta, distance, img.getWidth(), img.getHeight());
	img.clear();
	ren.start(cam, &img, scene);
	isEnter = true;

	time = GetTickCount() - time;
	if (time < 1000/fps)
		Sleep(1000/fps - time);
}

//-----------------------------------------------------------------------------
bool PtWindow::onMouse(Point_i pos, MouseType type) {
	const double distanceStep = 0.2;
	const double maxDistance = 100;
	const double moveCoeff = 0.008;

	switch (type) {
		case MOUSE_R_DOWN:
			func();
			break;
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
				changeCam();
			}
			break;
		case MOUSE_WHEEL_UP:
			if (distance > distanceStep) {
				distance -= distanceStep;
				changeCam();
			}
			break;
		case MOUSE_MOVE:
			if (moveRegime) {
				Point_i diff = last - pos;
				alpha -= diff.x * moveCoeff;
				beta += -diff.y * moveCoeff;
				if (beta < -pt::pi/2 + 0.05) beta = -pt::pi/2 + 0.05;
				if (beta > pt::pi/2 - 0.05)  beta = pt::pi/2 - 0.05;
				changeCam();
				last = pos;
			}
			break;
	}
	return true;
}

//-----------------------------------------------------------------------------
bool PtWindow::onResize(Rect rect, SizingType type) {
	long time = GetTickCount();
	isEnter = false;

	ImageBase img1 = ren.getCurrentImage(true);
	img1.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img1.size());
	ren.stop();
	img.resize(rect.x(), rect.y());
	onChangeCam(alpha, beta, distance, img.getWidth(), img.getHeight());
	img.clear();
	ren.start(cam, &img, scene);
	isEnter = true;

	time = GetTickCount() - time;
	if (time < 1000/fps)
		Sleep(1000/fps - time);

	return true;
}

//-----------------------------------------------------------------------------
bool PtWindow::onFocus(bool isKilled) {
	if (isKilled) 
		moveRegime = false;
	return true;
}

//-----------------------------------------------------------------------------
bool PtWindow::onMessage(int32u id, void* data) {
	if (!isStarted) {
		start();
		isStarted = true;
		setClientSize(getWindowSize());
	}
	if (id == WINDOWS_MESSAGE) {
		onMessageStruct msg = *(onMessageStruct*)(data);
		if (msg.msg == WM_TIMER && msg.wParam == 1220 && isEnter) {
			std::wstringstream sout;
			sout.precision(4);
			sout << std::setw(5) << (ren.getPercent() * 100) << L"%, alpha: " << alpha << L", beta: " << beta << L", distance: " << distance;
			setCaption(sout.str());

			ImageBase img1 = ren.getCurrentImage(false);
			img1.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img1.size());
			if (ren.getPercent() == 1)
				isEnter = false;
		}
	}
	return false;
}

}