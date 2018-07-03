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

		void render(const Camera& camera, Image& img, const Object& scene);

		Color computeColor(Ray ray, const Object& scene);
		Color computePixel(int x, int y, const Camera& camera, const Object& scene);

		void onStartRender(void) {}
		void onEveryLine(double percent) {}
		void onEndRendering(void) {}

		int samples;
		int maxDepth;
		int maxT;
	};

	//-------------------------------------------------------------------------
	class FitfulPathRenderer : public PathRenderer
	{
	public:
		FitfulPathRenderer(int maxDepth = 30,
						   double maxT = 100000);

		void init(Camera* camera, Image* img, Object* scene);
		/** Делает рендеринг некоторого количества семплов всего изображения, после этого возвращает копию изображения, которую можно нарисовать на экране. */
		Image renderStep(int samples);

	private:
		Camera* m_camera;
		Image* m_img;
		Object* m_scene;
		int m_samples;
	};

};

#endif