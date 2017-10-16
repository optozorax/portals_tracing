#include <pt/object/scene.h>

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
		m_current = 0;
		for (int i = 1; i < array.size(); ++i) {
			if (array[i]->intersect(ray, inter1, tMin, tMax)) {
				isIntersect = true;
				if (inter1.t < inter.t) {
					inter = inter1;
					m_current = i;
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
	return array[m_current]->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
}

};