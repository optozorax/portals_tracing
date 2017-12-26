#ifndef PT_THREAD_RENDERER_H
#define PT_THREAD_RENDERER_H

#include <pt/renderer.h>
#include <twg/twg.h>
#include <windows.h>
#include <special_travel_and_fill.h>

namespace pt
{

	//-------------------------------------------------------------------------
	class ThreadPartiallyRender
	{
	public:
		ThreadPartiallyRender(Renderer* renderer);

		void start(Camera* camera, Image* img, Object* scene);
		void pause(void);
		void extend(void);
		void stop(void);

		twg::ImageBase getCurrentImage(bool isStop);
		double getPercent(void);

	private:
		double m_percent;
		HANDLE m_thread;
		Image* m_img;
		Renderer* m_renderer;
		TravelPoints m_trvl;
		twg::Point_i m_size;
	};

};

#endif