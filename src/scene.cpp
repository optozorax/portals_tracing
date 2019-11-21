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
		bool isIntersect = false;
		inter.data.type = 0;
		for (int i = 0; i < array.size(); ++i) {
			if (array[i]->intersect(ray, inter1, tMin, tMax)) {
				if (inter1.t <= inter.t) {
					isIntersect = true;
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

};