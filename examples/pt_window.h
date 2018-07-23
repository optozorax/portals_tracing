#ifndef PT_PT_WINDOW_H
#define PT_PT_WINDOW_H

#include <twg/twg.h>
#include <twg/window/window_events.h>

#include <pt/pt.h>
#include <pt/pt2twg.h>

#include <pt/renderer/thread-renderer.h>

namespace pt
{

	class PtWindow : public twg::WindowEvents
	{
	public:
		PtWindow(twg::WindowType type, Renderer* ren, Camera* cam, Object* scene);
		virtual ~PtWindow();

		void start(void);

		virtual void onChangeCam(double alpha, double beta, double distance, int width, int height) = 0;
		virtual void createScene(void) = 0;
		virtual void func() {}

		//---------------------------------------------------------------------
		bool onMouse(twg::Point_i pos, twg::MouseType type);
		bool onResize(twg::Rect rect, twg::SizingType type);
		bool onFocus(bool isKilled);
		bool onMessage(twg::int32u id, void* data);

		void changeCam(void);
	private:
		Image					img;
		double					alpha;
		double					beta;
		double					distance;
		bool					moveRegime;
		bool 					isEnter;
		twg::Point_i			last;
		ThreadPartiallyRender	ren;
		Camera*					cam;
		Object*					scene;
		bool					isStarted;

		
		const int fps = 5;
	};

};

#endif