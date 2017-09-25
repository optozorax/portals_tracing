#ifndef PT_PATH_RENDERER_H
#define PT_PATH_RENDERER_H

#include <pt/renderer.h>

namespace pt
{

	//-------------------------------------------------------------------------
	/** Пускает на один пиксель количество лучей, заданное числом samples. */
	class PathRenderer1 : public Renderer
	{
	public:
		PathRenderer(int samples, 
					 int maxDepth = 30,
					 double maxT = 100000);

		void render(Camera& camera, Image& img, Object& scene);

		bool onStartRender(void) {}
		bool onEveryLine(float percent) {}
		bool onEndRendering(void) {}

		int samples;
		int maxDepth;
		int maxT;
	};

	//-------------------------------------------------------------------------
	/** Пускает на один пиксель количество лучей, заданное числом samples. При этом каждый раз при достижении поверхности снова запускает такое же количество лучей. Получается экспоненциальный рост. */
	class PathRenderer2 : public Renderer
	{
	public:
		PathRenderer(int samples, 
					 int maxDepth = 30,
					 double maxT = 100000);

		void render(Camera& camera, Image& img, Object& scene);

		bool onStartRender(void) {}
		bool onEveryLine(float percent) {}
		bool onEndRendering(void) {}

		int samples;
		int maxDepth;
		int maxT;
	};

};

#endif