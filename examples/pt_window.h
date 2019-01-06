#ifndef PT_PT_WINDOW_H
#define PT_PT_WINDOW_H

#include <twg/twg.h>
#include <twg/window/window_events.h>

#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/renderer/thread-renderer.h>

namespace pt
{

	/** Класс для просмотра 3Д сцен. Чтобы реализовать программу для просмотра 3Д сцен, необходимо наследоваться от этого класса, реализовать функции. */
	class PtWindow : public twg::WindowEvents
	{
	public:
		PtWindow(twg::WindowType type, StandardRenderer* ren);
		virtual ~PtWindow();
		
		// Переделать сцену
		void reCreateScene(void);

		// Функции, которые реализует наследуемый класс
		virtual void setFirstCam(double& focal, double& viewAngle, vec3& pos, vec3& lookAt, double& distance, double& alpha, double& beta) = 0; // Вызывается при инициализации камеры
		virtual void createScene(Scene& scene) = 0; // Вызывается, чтобы создать сцену
		virtual void onRightMouseClick(void) = 0; // Вызывается при нажатии правой кнопки мыши
		virtual void reCreateSceneUser(Scene& scene) = 0; // Вызывается, когда сцену надо обновить
	private:
		Image					img;
		twg::ImageBase			img1;

		ThreadPartiallyRender	ren;

		Scene					scene;

		PerspectiveCamera		cam;
		vec3					pos;
		vec3					lookAt;
		double					focal;
		double					viewAngle;
		double					alpha;
		double					beta;
		double					distance;

		bool					moveRegime;
		bool 					isEnter;
		twg::Point_i			last;
		bool					isStarted;
		
		const int fps = 5;

		//---------------------------------------------------------------------
		bool onMouse(twg::Point_i pos, twg::MouseType type);
		bool onResize(twg::Rect rect, twg::SizingType type);
		bool onFocus(bool isKilled);
		bool onMessage(twg::int32u id, void* data);

		void start(void);

		void changeCam(void);
	};

};

#endif