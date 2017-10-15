#include <pt/renderer/path-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
PathRenderer::PathRenderer(int samples, 
						   int maxDepth,
						   double maxT) : samples(samples), maxDepth(maxDepth), maxT(maxT) {
}

Color PathRenderer::computeColor(Ray ray, const Object& scene) {
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
			scattered.dir += randomSphere();
			scattered.dir.normalize();
			if (returned == SCATTER_END)
				break;
		} else {
			resultColor = resultColor * Color(0, 0, 0, 0);
			break;
		}
	}

	return resultColor;
}

//-----------------------------------------------------------------------------
void PathRenderer::render(Camera& camera, Image& img, Object& scene) {
	for (int i = 0; i < img.getWidth(); ++i) {
		for (int j = 0; j < img.getHeight(); ++j) {
			for (int i = 0; i < samples; ++i) {
				float x = i + random();
				float y = j + random();
				Ray ray = camera.getRay(x, y);
				img(i, j) = computeColor(ray, scene);
			}
			img(i, j) /= samples;
		}
	}
}

};