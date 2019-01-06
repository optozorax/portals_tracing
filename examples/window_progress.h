#ifndef PT_WINDOW_PROGRESS_H
#define PT_WINDOW_PROGRESS_H

#include <twg/twg.h>
#include <twg/window/window_events.h>

#include <pt/pt.h>
#include <pt/pt2twg.h>

#include <pt/renderer/thread-renderer.h>

namespace pt
{

	/** Тип окна, который показывает прогресс рендеринга изображения с любым типом рендеринга, любой камерой, любой сценой. Для этого всего необходимо передать окну размеры изображения, указатель на камеру, указатель на сцену, имя файла куда сохранить после рендеринга. После рендеринга окно закроется. */
	class WindowProgress : public twg::WindowEvents
	{
	public:
		WindowProgress(twg::Point_i size, StandardRenderer* ren, Camera* cam, Object* scene, std::string imageName);
		~WindowProgress();

		//---------------------------------------------------------------------
		bool onMessage(twg::int32u id, void* data);
	protected:
		twg::ImageBase			img1;

	private:
		Image					img;
		ThreadPartiallyRender	ren;
		Camera*					cam;
		Object*					scene;

		std::string				imageName;

		bool					isStarted;
		bool					sizeInstalled;

		long					time;
	};

};

#endif