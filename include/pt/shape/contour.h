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
		Contour(std::vector<vec3> array, double thick, bool isClosed, Material_ptr material) : Shape(material) {
			assign(array, thick, isClosed);
		}

		void assign(const std::vector<vec3>& array, double thick, bool isClosed);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
	private:
		Scene					m_scene;
	};

	inline Object_ptr makeContour(std::vector<vec3> array, double thick, bool isClosed, Material_ptr material) { return Object_ptr(new Contour(array, thick, isClosed, material)); }

};

#endif