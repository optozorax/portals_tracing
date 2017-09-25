#ifndef PT_RAY_RENDERER_H
#define PT_RAY_RENDERER_H

#include <vector>
#include <pt/renderer.h>

namespace pt
{

	struct PointLightSource
	{
		Vector pos;
		Color clr;
	};

	//-------------------------------------------------------------------------
	/** Испускает из камеры ровно один луч, когда луч достигает тела, проверяет, достижим ли источник света, если да, то закрашивает его с учетом источника освещения, иначе с учетом неба или вообще полностью темным закрашивает. */
	class RayRenderer
	{
	public:
		RayRenderer(double maxT = 100000);

		void render(Camera& camera, Image& img, Object& scene);

		bool onStartRender(void) {}
		bool onEveryLine(float percent) {}
		bool onEndRendering(void) {}

		std::vector<PointLightSource> luminaries;
		Color skyColor;
		int samples;
		int maxDepth;
		int maxT;
	};

};

#endif