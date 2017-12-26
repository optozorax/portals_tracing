#include <pt/renderer/thread-renderer.h>
#include <special_travel_and_fill.h>
#include <twg/twg.h>
#include <pt/pt2twg.h>
#include <windows.h>

namespace pt
{

//-----------------------------------------------------------------------------
ThreadPartiallyRender::ThreadPartiallyRender(Renderer* renderer) : m_renderer(renderer) {
	m_thread = 0;
}

//-----------------------------------------------------------------------------
struct Information {
	Camera* camera;
	Image* img;
	Object* scene;
	double* percent;
	TravelPoints* trvl;
	Renderer* renderer;
};

//-----------------------------------------------------------------------------
bool waitForCreate = false;
DWORD WINAPI renderingThread(LPVOID data) {
	using namespace twg;
	Information inf = *(Information*)(data);
	delete data;

	// Алгоритм
	Point_i p;
	for (int i = 0; i < inf.trvl->size(); ++i) {
		*(inf.percent) = double(i)/inf.trvl->size();
		p = (inf.trvl)->get(i);
		(*(inf.img))(p.x, p.y) = (inf.renderer)->computePixel(p.x, p.y, *(inf.camera), *(inf.scene));
	}

	return 0;
}

//-----------------------------------------------------------------------------
void ThreadPartiallyRender::start(Camera* camera, Image* img, Object* scene) {
	using namespace twg;
	if (m_thread != 0)
		TerminateThread(m_thread, 0);

	Point_i newSize(img->getWidth(), img->getHeight());
	if (newSize != m_size)
		m_trvl.compute(newSize);
	m_size = newSize;

	Information* inf = new Information;
	inf->camera = camera;
	inf->img = img;
	inf->scene = scene;
	inf->percent = &m_percent;
	inf->trvl = &m_trvl;
	inf->renderer = m_renderer;
	m_thread = CreateThread(NULL, 0, &renderingThread, inf, 0, NULL);
	if (m_thread == 0)
		exit(0);
	m_img = img;
}

//-----------------------------------------------------------------------------
void ThreadPartiallyRender::stop(void) {
	TerminateThread(m_thread, 0);
}

//-----------------------------------------------------------------------------
void ThreadPartiallyRender::pause(void) {
	SuspendThread(m_thread);
}

//-----------------------------------------------------------------------------
void ThreadPartiallyRender::extend(void) {
	ResumeThread(m_thread);
}

//-----------------------------------------------------------------------------
twg::ImageBase ThreadPartiallyRender::getCurrentImage(bool isPause = true) {
	using namespace twg;

	pause();

	ImageBase img(Point_i(1, 1));
	Image img3(m_img);
	img3.colorCorrection();
	toTwgImage(img3, img);
	FillHalfrend::fill(img);

	if (!isPause)
		extend();

	return img;
}

//-----------------------------------------------------------------------------
double ThreadPartiallyRender::getPercent(void) {
	pause();
	double percent = m_percent;
	extend();

	return percent;
}

};