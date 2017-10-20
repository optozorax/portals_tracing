#include <pt/renderer/ray-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
RayRenderer::RayRenderer(int antialiasing, double maxDepth, double maxT) : maxDepth(maxDepth), maxT(maxT), antialiasing(antialiasing) {
}

//-----------------------------------------------------------------------------
inline Color RayRenderer::computeColor(Ray ray, const Object& scene) {
	Color lightColor = skyColor;
	Color materialColor(1, 1, 1, 1);
	Intersection inter;
	Color clrAbsorbtion;
	Ray scattered;
	double diffusion;
	ScatterType returned;
	
	bool haveMaterial = false;
	for (int i = 0; i < maxDepth; ++i) {
		if (scene.intersect(ray, inter, 0, maxT)) {
			haveMaterial = true;
			returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
			materialColor = clrAbsorbtion * materialColor;
			ray = scattered;
			scattered.dir.normalize();
			if (returned == SCATTER_END || returned == SCATTER_RAYTRACING_END)
				break;
		} else
			break;
	}

	Vector normal = inter.normal;
	if (haveMaterial) {
		for (int i = 0; i < luminaries.size(); ++i) {
			ray.dir = luminaries[i].pos - ray.pos;
			ray.dir.normalize();
			double cosine = dot(normal, ray.dir);
			if (cosine > 0) {
				if (scene.intersect(ray, inter, 0, maxT)) {
					if (inter.t > (luminaries[i].pos - ray.pos).getLength()) 
						lightColor += luminaries[i].clr * cosine;
				} else
					lightColor += luminaries[i].clr * cosine;
			}
		}
	}

	return materialColor * lightColor;
}

//-----------------------------------------------------------------------------
void RayRenderer::render(Camera& camera, Image& img, Object& scene) {
	onStartRender();
	for (int i = 0; i < img.getWidth(); ++i) {
		onEveryLine(double(i)/img.getWidth());
		for (int j = 0; j < img.getHeight(); ++j) {
			for (int ki = 0; ki < antialiasing; ++ki) {
				for (int kj = 0; kj < antialiasing; ++kj) {
					double x = i + double(ki)/antialiasing;
					double y = j + double(kj)/antialiasing;
					Ray ray = camera.getRay(x, y);
					img(i, j) += computeColor(ray, scene);	
				}
			}
			if (antialiasing != 0.1)
				img(i, j) /= antialiasing * antialiasing;
		}
	}
	onEndRendering();
}

};