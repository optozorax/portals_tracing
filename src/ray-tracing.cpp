#include <pt/renderer/ray-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
RayRenderer::RayRenderer(int antialiasing, double maxDepth, double maxT) : maxDepth(maxDepth), maxT(maxT), antialiasing(antialiasing) {
}

//-----------------------------------------------------------------------------
inline Color RayRenderer::computeColor(Ray ray, const Object& scene) {
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

			// Если полигон полупрозрачный, то его цвет будет комбинацией двух лучей, сложенных с учетом прозрачности
			if (clrAbsorbtion.a != 1) {
				// Изменяем и сохраняем глубину рендерера, чтобы он не зациклился
				int temp = maxDepth;
				maxDepth -= i;

				double opaque = clrAbsorbtion.a;
				clrAbsorbtion.a = 1;

				// Получаем цвет луча, который пойдет сквозь материал
				Ray through;
				through.pos = scattered.pos;
				through.dir = ray.dir;
				through.pos += through.dir * 0.001;
				Color ray2color = computeColor(through, scene);

				scattered.pos += scattered.dir * 0.001;

				// Получаем цвет луча, который пошел бы обычным путем
				Color ray1color;
				if (returned == SCATTER_END || returned == SCATTER_RAYTRACING_END) {
					ray1color = computeLightColor(scattered, scene, true, inter.normal);
					ray1color = clrAbsorbtion * ray1color;
				} else
					ray1color = computeColor(scattered, scene);

				// Совмещаем два цвета с учетом прозрачности
				Color rayColor;
				rayColor.a = 1;
				rayColor.r = opaque * ray1color.r + (1 - opaque) * ray2color.r;
				rayColor.g = opaque * ray1color.g + (1 - opaque) * ray2color.g;
				rayColor.b = opaque * ray1color.b + (1 - opaque) * ray2color.b;

				maxDepth = temp;
				return rayColor;
			}

			scattered.pos += scattered.dir * 0.001;

			materialColor = clrAbsorbtion * materialColor;
			ray = scattered;
			scattered.dir.normalize();
			if (returned == SCATTER_END || returned == SCATTER_RAYTRACING_END)
				break;
		} else
			break;
	}

	Color returnColor;
	Color lightColor = computeLightColor(ray, scene, haveMaterial, inter.normal);
	returnColor = materialColor * lightColor;
	return returnColor;
}

//-----------------------------------------------------------------------------
Color RayRenderer::computeLightColor(Ray ray, const Object& scene, bool haveMaterial, Vector normal) {
	Intersection inter;
	Vector rayPos = ray.pos;
	inter.normal = normal;

	// Инициируем освещенность глобальным освещением - цветом неба
	Color lightColor = skyColor;
	if (haveMaterial) {
		for (int i = 0; i < luminaries.size(); ++i) {

			// Получаем направление к источнику света
			ray.dir = luminaries[i].pos - ray.pos;
			ray.dir.normalize();

			// Данное значение показывает насколько много света освещает тело, следовательно оно влияет на освещенность.
			double cosine = dot(normal, ray.dir);
			if (cosine > 0) {
				Color current(1, 1, 1, 1);
				again:
				if (scene.intersect(ray, inter, 0, maxT)) {
					if (inter.t > (luminaries[i].pos - ray.pos).getLength())
						// Если при взгляде на точечный источник света на пути к нему нету объектов(пересечение со сценой дальше, чем источник света), то данный источник света освещает объект, и прибавляем его к цвету освещения
						lightColor += current * luminaries[i].clr * cosine;
					else {
						Color clrAbsorbtion;
						Ray scattered;
						double diffusion;
						ScatterType returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
						// Если на пути к источнику света есть полупрозрачный полигон, то делаем особые вычисления, чтобы вычислить сколько света по каждому из каналов пройдет от данного источника света, затем смещаем луч, чтобы посчитать что есть дальше
						if (clrAbsorbtion.a < 1) {
							current.r *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.r * (1 - clrAbsorbtion.a)));
							current.g *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.g * (1 - clrAbsorbtion.a)));
							current.b *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.b * (1 - clrAbsorbtion.a)));
							ray.pos = inter.pos + ray.dir * 0.001;
							goto again;
						}
					}
				} else
					// Если при взгляде на точечный источник света на пути к нему нету объектов, то данный источник света освещает объект, и прибавляем его к цвету освещения
					lightColor += current * luminaries[i].clr * cosine;

				// Возвращаем предыдущее положение луча, так как мы его могли менять во время путешествия сквозь полупрозрачные объекты
				ray.pos = rayPos;
			}
		}
	}

	return lightColor;
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
			img(i, j) /= antialiasing * antialiasing;
		}
	}
	onEndRendering();
}

};