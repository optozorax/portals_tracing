#ifndef PT_RAY_RENDERER_H
#define PT_RAY_RENDERER_H

#include <vector>
#include <pt/renderer.h>

namespace pt
{

	struct PointLightSource
	{
		PointLightSource(Vector pos, Color clr) : pos(pos), clr(clr) {}

		Vector pos;
		Color clr;
	};

	//-------------------------------------------------------------------------
	/** Испускает из камеры ровно один луч, когда луч достигает тела, проверяет, достижим ли источник света, если да, то закрашивает его с учетом источника освещения, иначе с учетом неба или вообще полностью темным закрашивает. */
	class RayRenderer
	{
	public:
		RayRenderer(int antialiasing = 1, double maxDepth = 30, double maxT = 100000);

		void render(Camera& camera, Image& img, Object& scene);

		Color computeColor(Ray ray, const Object& scene);

		void onStartRender(void) {}
		void onEveryLine(float percent) {}
		void onEndRendering(void) {}

		std::vector<PointLightSource> luminaries;
		Color skyColor;
		int maxDepth;
		int maxT;
		double antialiasing;
	};

};

#endif