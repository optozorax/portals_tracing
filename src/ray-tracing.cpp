#include <pt/renderer/ray-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
RayRenderer::RayRenderer(double maxDepth, double maxT) : maxDepth(maxDepth), maxT(maxT) {

}

Color RayRenderer::computeColor(Ray ray, const Object& scene) {
	Color resultColor = Color(1, 1, 1, 1);
	Intersection inter;
	Color clrAbsorbtion;
	Ray scattered;
	double diffusion;
	ScatterType returned;

	for (int i = 0; i < maxDepth; ++i) {
		if (scene.intersect(ray, inter, 0, maxT)) {
			returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
			resultColor = resultColor * clrAbsorbtion;
			ray = scattered;
			scattered.dir.normalize();
			if (returned == SCATTER_END || returned == SCATTER_RAYTRACING_END)
				break;
		} else {
			resultColor = resultColor * Color(0, 0, 0, 0);
			break;
		}
	}

	for (int i = 0; i < luminaries.size(); ++i) {
		ray.dir = luminaries[i].pos - ray.pos;
		ray.dir.normalize();
		if (scene.intersect(ray, inter, 0, maxT)) {
			if (inter.t > (luminaries[i].pos - ray.pos).getLength()) 
				resultColor = resultColor * luminaries[i].clr;
		} else
			resultColor = resultColor * luminaries[i].clr;
	}

	return resultColor;
}

//-----------------------------------------------------------------------------
void RayRenderer::render(Camera& camera, Image& img, Object& scene) {
	for (int i = 0; i < img.getWidth(); ++i) {
		for (int j = 0; j < img.getHeight(); ++j) {
			Ray ray = camera.getRay(i, j);
			img(i, j) = computeColor(ray, scene);
		}
	}
}

};