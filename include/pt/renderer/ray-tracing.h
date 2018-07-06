#ifndef PT_RAY_RENDERER_H
#define PT_RAY_RENDERER_H

#include <vector>
#include <pt/shape/portals.h>
#include <pt/renderer.h>

namespace pt
{

	struct PointLightSource
	{
		PointLightSource(vec3 pos, Color clr) : pos(pos), clr(clr) {}

		vec3 pos;
		Color clr;
	};

	//-------------------------------------------------------------------------
	/** Испускает из камеры ровно один луч, когда луч достигает тела, проверяет, достижим ли источник света, если да, то закрашивает его с учетом источника освещения, иначе с учетом неба или вообще полностью темным закрашивает. */
	class RayRenderer : public Renderer
	{
	public:
		RayRenderer(int antialiasing = 1, double maxDepth = 30, double maxT = 100000);

		void render(const Camera& camera, Image& img, const Object& scene);

		Color computeColor(Ray ray, const Object& scene);
		Color computePixel(int x, int y, const Camera& camera, const Object& scene);
		
		Color computeLightColor(Ray ray, const Object& scene, vec3 normal);
		Color rayPassage(vec3 pos, vec3 normal, vec3 lightPos, Color lightColor, const Object& scene, int depth, double tMax);

		void onStartRender(void) {}
		void onEveryLine(double percent) {}
		void onEndRendering(void) {}

		std::vector<PointLightSource> luminaries;
		std::vector<Portals*> portals;
		Color skyColor;
		int maxDepth;
		int maxT;
		double antialiasing;
	};

};

#endif