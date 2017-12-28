#include <pt/shape/contour.h>

namespace pt
{

//-----------------------------------------------------------------------------
void Contour::assign(const std::vector<Vector>& array, double thick, bool isClosed) {
	for (int i = 0; i < array.size(); ++i)
		m_spheres.push_back(Sphere(array[i], thick, nullptr));
	for (int i = 0; i < array.size()-1; ++i)
		m_cylinders.push_back(Cylinder(array[i], array[i+1], thick, nullptr));
	if (isClosed)
		m_cylinders.push_back(Cylinder(array[array.size() - 1], array[0], thick, nullptr));

	for (int i = 0; i < array.size(); ++i) {
		m_scene.array.push_back(&m_spheres[i]);
		if ((i == array.size()-1 && isClosed) || (i != array.size()-1))
			m_scene.array.push_back(&m_cylinders[i]);
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