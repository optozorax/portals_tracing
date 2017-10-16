#ifndef PT_PATH_RENDERER_H
#define PT_PATH_RENDERER_H

#include <pt/renderer.h>

namespace pt
{

	//-------------------------------------------------------------------------
	/** Пускает на один пиксель количество лучей, заданное числом samples. */
	class PathRenderer : public Renderer
	{
	public:
		PathRenderer(int samples, 
					 int maxDepth = 30,
					 double maxT = 100000);

		void render(Camera& camera, Image& img, Object& scene);

		Color computeColor(Ray ray, const Object& scene);

		void onStartRender(void) {}
		void onEveryLine(float percent) {}
		void onEndRendering(void) {}

		int samples;
		int maxDepth;
		int maxT;
	};

};

#endif