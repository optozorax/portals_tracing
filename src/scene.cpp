#include <iostream>

#include <pt/object/scene.h>
#include <pt/shape/polygon.h>
#include <pt/object/texture_polygon.h>
#include <pt/material/scatter.h>
#include <pt/shape/portals.h>
#include <pt/pt2easybmp.h>
#include <stb_image.h>

namespace pt
{

//-----------------------------------------------------------------------------
bool Scene::intersect(const Ray& ray, 
					  Intersection& inter, 
					  double tMin, 
					  double tMax) const {
	if (array.size() > 0) {
		Intersection inter1;
		inter.t = tMax;
		bool isIntersect = array[0]->intersect(ray, inter, tMin, tMax);;
		inter.data.type = 0;
		for (int i = 1; i < array.size(); ++i) {
			if (array[i]->intersect(ray, inter1, tMin, tMax)) {
				isIntersect = true;
				if (inter1.t <= inter.t) {
					inter = inter1;
					inter.data.type = i;
				}
			}
		}
		return isIntersect;
	} else
		return false;
}

//-----------------------------------------------------------------------------
ScatterType Scene::scatter(const Ray& ray,
					const Intersection& inter,
					Color& clrAbsorbtion,
					Ray& scattered,
					double& diffusion) const {
	return array[inter.data.type]->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
}

//-----------------------------------------------------------------------------
void Scene::add(Object_ptr obj) {
	array.push_back(obj);
}

//-----------------------------------------------------------------------------
Scene loadFrame(const scene::Frame& frame) {
	Scene result;
	// Считываем раскрашенные полигоны
	for (auto& i : frame.colored_polygons) {
		result.array.push_back(makePolygon(
			i.polygon,
			i.crd,
			makeScatter(Color(i.color.x, i.color.y, i.color.z))
		));
	}

	// Считываем порталы
	for (auto& i : frame.portals) {
		result.array.push_back(makePortals(
			i.crd2, i.crd1,
			i.polygon,
			makeScatter(Color(i.color1.x, i.color1.y, i.color1.z)),
			makeScatter(Color(i.color2.x, i.color2.y, i.color2.z))
		));
	}

	// Считываем текстуры
	std::vector<Image_ptr> images;
	for (auto& i : frame.textures) {
		Image_ptr img(new Image(1, 1));
		loadAsPng(*img, i.filename);
		images.push_back(img);
	}

	// Считываем текстурированные полигоны
	for (auto& i : frame.textured_polygons) {
		space2 line1, line2, newSpace;

		// Подбираем такие 3 точки, чтобы получилась невырожденная система координат
		for (int j = 0; j < i.polygon.size() - 2; j++) {
			auto a = i.polygon[j];
			auto b = i.polygon[j+1];
			auto c = i.polygon[j+2];

			auto at = i.tex_coords[j];
			auto bt = i.tex_coords[j+1];
			auto ct = i.tex_coords[j+2];

			line1 = space2(b - a, c - a, a);
			line2 = space2(bt - at, ct - at, at);

			newSpace = combine(invert(line2), line1);

			// Проверяем на невырожденность
			for (int k = 0; k < i.polygon.size(); k++) {
				if (!isNear(i.tex_coords[k], newSpace.to(i.polygon[k]))) {
					goto next_iteration;
				}
			}

			// Если проверка не разу не случилась, то нам подходит эта система координат
			goto end_cycle;

			next_iteration:;
		}

		// В нормальном случае мы должны перескочить этот участок
		std::cout << "You have line figure with texture. That's bad." << std::endl;

		end_cycle:
		result.array.push_back(makeTexturePolygon(
			i.polygon,
			i.crd,
			images[i.texture_id],
			newSpace
		));
	}

	return result;
}

};