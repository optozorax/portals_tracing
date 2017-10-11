#ifndef PT_RENDERER_H
#define PT_RENDERER_H

#include <pt/camera.h>
#include <pt/image.h>
#include <pt/object.h>

namespace pt
{

	class Renderer
	{
	public:
		virtual ~Renderer() {}
		virtual void render(Camera& camera, Image& img, Object& scene) = 0;

		/** Функции, необходимые для отслеживания прогресса рендеринга. Необходимы, так как рендеринг может длиться очень долго. */
		virtual bool onStartRender(void) = 0;
		virtual bool onEveryLine(float percent) = 0;
		virtual bool onEndRendering(void) = 0;

	};

};

#endif