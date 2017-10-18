#ifndef PT_PT_WINDOW_H
#define PT_PT_WINDOW_H

#include <twg/twg.h>
#include <twg/window/window_events.h>

#include <pt/pt.h>
#include <pt/pt2twg.h>

namespace pt
{

	class PtWindow : public twg::WindowEvents
	{
	public:
		PtWindow(twg::WindowType type);

		void start(void);

		virtual void onChangeCam(double alpha, double beta, double distance) = 0;
		virtual void onDraw(Image& img) = 0;
		virtual void createScene(void) = 0;

		//---------------------------------------------------------------------
		bool onMouse(twg::Point_i pos, twg::MouseType type);
		bool onResize(twg::Rect rect, twg::SizingType type);
		bool onFocus(bool isKilled);
	private:
		pt::Image				img;
		twg::ImageBase			img2;
		double					alpha;
		double					beta;
		double					distance;
		bool					moveRegime;
		twg::Point_i			last;

		void redraw(void);
	};

};

#endif