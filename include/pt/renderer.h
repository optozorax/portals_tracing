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
		virtual void render(const Camera& camera, Image& img, const Object& scene) = 0;

		virtual Color computeColor(Ray ray, const Object& scene) = 0;
		virtual Color computePixel(int x, int y, const Camera& camera, const Object& scene) = 0;

		/** Функции, необходимые для отслеживания прогресса рендеринга. Необходимы, так как рендеринг может длиться очень долго. */
		virtual void onStartRender(void) = 0;
		virtual void onEveryLine(double percent) = 0;
		virtual void onEndRendering(void) = 0;

	};

};

#endif