#include <pt/renderer/ray-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
RayRenderer::RayRenderer(int antialiasing, double maxDepth, double maxT) : maxDepth(maxDepth), maxT(maxT), antialiasing(antialiasing) {
}

//-----------------------------------------------------------------------------
Color RayRenderer::computePixel(int x, int y, const Camera& cam, const Object& scene) {
	Color clr(0, 0, 0, 0);
	for (int ki = 0; ki < antialiasing; ++ki) {
		for (int kj = 0; kj < antialiasing; ++kj) {
			double x1 = x + double(ki)/antialiasing;
			double y1 = y + double(kj)/antialiasing;
			Ray ray = cam.getRay(x1, y1);
			clr += computeColor(ray, scene);
		}
	}
	clr /= antialiasing * antialiasing;
	return clr;
}

//-----------------------------------------------------------------------------
Color RayRenderer::computeColor(Ray ray, const Object& scene) {
	Color materialColor(1, 1, 1, 1);
	Intersection inter;
	Color clrAbsorbtion;
	Ray scattered;
	double diffusion;
	ScatterType returned;
	
	bool haveMaterial = false;
	for (int i = 0; i < maxDepth; ++i) {
		if (scene.intersect(ray, inter, 0, maxT)) {
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
					ray1color = computeLightColor(scattered, scene, inter.normal);
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
			if (returned == SCATTER_END || returned == SCATTER_RAYTRACING_END) {
				haveMaterial = true;
				break;
			}
		} else
			break;
	}

	Color returnColor;
	Color lightColor = skyColor;
	if (haveMaterial) 
		lightColor += computeLightColor(ray, scene, inter.normal);
	returnColor = materialColor * lightColor;
	return returnColor;
}

//-----------------------------------------------------------------------------
Color RayRenderer::computeLightColor(Ray ray, const Object& scene, Vector normal) {
	Color lightColor(0, 0, 0);

	for (int i = 0; i < luminaries.size(); ++i) {
		double tMax = (luminaries[i].pos - ray.pos).getLength();
		lightColor += rayPassage(ray.pos, normal, luminaries[i].pos, luminaries[i].clr, scene, 3, tMax);
	}

	return lightColor;
}

//-----------------------------------------------------------------------------
Color RayRenderer::rayPassage(Vector pos, Vector normal, Vector lightPos, Color lightColor, const Object& scene, int depth, double tMax) {
	if (depth == 0)
		return Color(0, 0, 0, 0);

	Color result(0, 0, 0, 0);
	// Перебираем все порталы
	for (int i = 0; i < portals.size(); ++i) {
		// Создаем копию текущего портала и копию этого же портала, только порталы расположены в обратном порядке. Чтобы обработать как прямой порядок следования порталов, так и обратный.
		Portals current(portals[i]->p1, portals[i]->p2, portals[i]->poly, nullptr, nullptr);
		CoordSystem coords1 = current.p2;
		coords1.k = -coords1.k;
		CoordSystem coords2 = current.p1;
		coords2.k = -coords2.k;
		Portals currentInverse(coords1, coords2, current.poly, nullptr, nullptr);

		// Здесь обрабатывается один портал и источник света
		auto func = [&] (Portals* current) -> Color {
			// Получаем положение телепортированного источника света 1->2
			Vector lightTeleportPos = teleportVector(current->p1, current->p2, lightPos);

			Ray ray;
			ray.pos = pos;
			ray.dir = lightTeleportPos - pos;
			ray.dir.normalize();

			double cosine = dot(ray.dir, normal);

			Color result1(0, 0, 0, 0);
			Intersection inter;
			// Если луч, соединяющий текущее положение и телепортированный источник света точно пересекает 2 портал, при этом таким образом, что луч входит в этот портал, то 
			if (cosine > 0 && dot(ray.dir, current->p2.k) < 0 && current->pg2.intersect(ray, inter, 0, tMax)) {
				// Делаем проход луча от текущего положения до 2, при этом источник света телепортированный. Глубина-1
				Color clr1 = rayPassage(ray.pos, ray.dir, lightTeleportPos, lightColor, scene, depth-1, inter.t - 0.01);
					
				// Телепортируем текущее положение 2->1
				Ray ray1;
				ray1.pos = teleportVector(current->p2, current->p1, pos);
				ray1.dir = lightPos - ray1.pos;
				ray1.dir.normalize();
				ray1.pos += ray1.dir * (inter.t + 0.01);

				// Делаем проход луча от 1 до истинного положения источника света. Глубина-1. Цвет источника освещения уже изменен первым проходом
				Color clr2 = rayPassage(ray1.pos, ray1.dir, lightPos, clr1, scene, depth-1, tMax - inter.t);

				// Прибавляем их к результирующему цвету умноженному на новый косинус
				result1 = clr2 * cosine;
			}

			return result1;
		};

		// Обрабатываем прямой и обратный порядок следования порталов
		result += func(&current);
		result += func(&currentInverse);
	}

	Ray ray;
	ray.pos = pos;
	ray.dir = lightPos - pos;
	ray.dir.normalize();
	double cosine = dot(ray.dir, normal);
	Intersection inter;
	if (cosine > 0) {
		again:
		// Проверяем, есть ли на пути к источнику освещения какие-либо объекты
		if (scene.intersect(ray, inter, 0, tMax)) {
			Color clrAbsorbtion;
			Ray scattered;
			double diffusion;
			ScatterType returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
			// Если на пути к источнику света есть полупрозрачный полигон, то делаем особые вычисления, чтобы вычислить сколько света по каждому из каналов пройдет от данного источника света, затем смещаем луч, чтобы посчитать что есть дальше
			if (clrAbsorbtion.a < 1) {
				lightColor.r *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.r * (1 - clrAbsorbtion.a)));
				lightColor.g *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.g * (1 - clrAbsorbtion.a)));
				lightColor.b *= (1 - clrAbsorbtion.a*(1 - clrAbsorbtion.b * (1 - clrAbsorbtion.a)));
				ray.pos = inter.pos + ray.dir * 0.001;
				tMax -= inter.t;
				goto again;
			}
		} else
			// Если нет, то прибавляем к результирующему цвету источник освещения
			result += lightColor * cosine;
	}

	return result;
}

//-----------------------------------------------------------------------------
void RayRenderer::render(const Camera& camera, Image& img, const Object& scene) {
	onStartRender();
	for (int i = 0; i < img.getWidth(); ++i) {
		onEveryLine(double(i)/img.getWidth());
		for (int j = 0; j < img.getHeight(); ++j) {
			img(i, j) = computePixel(i, j, camera, scene);
		}
	}
	onEndRendering();
}

};