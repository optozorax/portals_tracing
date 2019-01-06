#include <sstream>
#include <iomanip>
#include <pt/pt2easybmp.h>
#include "window_progress.h"

namespace pt
{

using namespace twg;

//-----------------------------------------------------------------------------
WindowProgress::WindowProgress(Point_i size, StandardRenderer* ren, Camera* cam, Object* scene, std::string imageName) : 
	WindowEvents(WindowType(stdIcon, Point_i(100, 100), Point_i(200, 200), stdMin, stdMax, L"Drawing progress", WINDOW_STANDART)),
	img(size.x, size.y),
	ren(ren),
	cam(cam),
	scene(scene),
	isStarted(false),
	sizeInstalled(false),
	imageName(imageName),
	img1({1, 1}) {
}

//-----------------------------------------------------------------------------
WindowProgress::~WindowProgress() {
	ren.stop();
}

//-----------------------------------------------------------------------------
bool WindowProgress::onMessage(int32u id, void* data) {
	if (!isStarted) {
		// Здесь надо начать работу окна
		ren.start(cam, &img, scene);
		SetTimer(getHwnd(), 1220, 1000, NULL);
		time = GetTickCount();
		isStarted = true;
	}

	if (id == WINDOWS_MESSAGE) {
		onMessageStruct msg = *(onMessageStruct*)(data);
		if (msg.msg == WM_TIMER && msg.wParam == 1220) {
			if (!sizeInstalled) {
				// Здесь надо задать размеры, если они не заданы
				setClientSize(Point_i(img.getWidth(), img.getHeight()));
				setMinSize(getWindowSize());
				setMaxSize(getWindowSize());
				sizeInstalled = true;
			}

			// Здесь сработал таймер, надо перерисовать изображение на экране
			std::wstringstream sout;
			sout.precision(2);
			double percent = ren.getPercent() * 100.0;
			double time1 = (GetTickCount() - time)/1000.0;
			double left = time1/percent * 100.0 - time1;
			sout << percent << L"%, time: " << (int)(time1) << L", left: " << (int)(left);
			setCaption(sout.str());

			ren.getCurrentImage(img1, false);
			img1.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img1.size());

			if (ren.getPercent() == 1) {
				saveAsBmp(img, imageName);
				PostMessage(getHwnd(), WM_CLOSE, 0, 0);
			}

			return true;
		}
	}

	return false;
}

}