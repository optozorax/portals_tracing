#include <pt/shape/contour.h>

namespace pt
{

//-----------------------------------------------------------------------------
void Contour::assign(const std::vector<Vector>& array, double thick) {
	for (int i = 0; i < array.size(); ++i) {
		m_spheres.push_back(new TrComplex(Vector(thick, thick, thick)/2.0, Vector(0), array[i], &m_sphere));
		m_scene.array.push_back(m_spheres[i]);
	}
	for (int i = 0; i < array.size() - 1; ++i) {
		Vector p = array[i+1] - array[i];
		Vector angles(atan2(p.y, sqrt(p.x*p.x + p.z*p.z)), atan2(p.x, p.z), 0);
		m_cylinders.push_back(new TrComplex(Vector(thick, thick, p.getLength()), angles, (array[i+1] + array[i])/2.0, &m_cylinder));
		m_scene.array.push_back(m_cylinders[m_cylinders.size() - 1]);
	}
	Vector p = array[array.size() - 1] - array[0];
	Vector angles(atan2(p.y, sqrt(p.x*p.x + p.z*p.z)), atan2(p.x, p.z), 0);
	m_cylinders.push_back(new TrComplex(Vector(thick, thick, p.getLength()), angles, (array[array.size() - 1] + array[0])/2.0, &m_cylinder));
	m_scene.array.push_back(m_cylinders[array.size() - 1]);
}

//-----------------------------------------------------------------------------
bool Contour::intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const {
	return m_scene.intersect(ray, inter, tMin, tMax);
}

};