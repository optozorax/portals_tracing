#ifndef PT_CONTOUR_H
#define PT_CONTOUR_H

#include <vector>
#include <pt/object.h>
#include <pt/shape/sphere.h>
#include <pt/shape/cylinder.h>
#include <pt/object/scene.h>
#include <pt/transformation.h>

namespace pt
{

	class Contour : public Shape
	{
	public:
		Contour(std::vector<Vector> array, double thick, Material* material) : m_sphere(material), m_cylinder(material), Shape(material) {
			assign(array, thick);
		}

		void assign(const std::vector<Vector>& array, double thick);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		Scene					m_scene;
		Sphere					m_sphere;
		Cylinder				m_cylinder;
		std::vector<TrComplex*>	m_spheres;
		std::vector<TrComplex*>	m_cylinders;
	};

};

#endif