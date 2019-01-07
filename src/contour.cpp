#include <pt/shape/contour.h>

namespace pt
{

//-----------------------------------------------------------------------------
void Contour::assign(const std::vector<vec3>& array, double thick, bool isClosed) {
	for (int i = 0; i < array.size(); ++i) {
		m_scene.array.push_back(makeSphere(array[i], thick, nullptr));
		if ((i == array.size()-1 && isClosed) || (i != array.size()-1))
			m_scene.array.push_back(makeCylinder(array[i], array[i+1], thick, nullptr));
	}
}

//-----------------------------------------------------------------------------
bool Contour::intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const {
	return m_scene.intersect(ray, inter, tMin, tMax);
}

};