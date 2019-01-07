#ifndef PT_CUBEMAP_H
#define PT_CUBEMAP_H

#include <string>
#include <EasyBMP.h>
#include <pt/object.h>

namespace pt
{

	class CubeMap : public Object
	{
	public:
		CubeMap(std::string bmp);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;
	private:
		BMP m_image;
		int m_size;
	};

	inline Object_ptr makeCubeMap(std::string bmp) { return Object_ptr(new CubeMap(bmp)); }

};

#endif