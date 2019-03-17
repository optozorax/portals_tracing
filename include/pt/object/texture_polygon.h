#ifndef PT_TEXTURE_POLYGON_H
#define PT_TEXTURE_POLYGON_H

#include <pt/object.h>
#include <pt/image.h>
#include <pt/shape/polygon.h>

namespace pt
{

	class TexturePolygon : public Object
	{
	public:
		TexturePolygon(const std::vector<vec2>& polygon, crd3 coords, Image_ptr img, const space2& tr);

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
		Image_ptr img;

		std::vector<vec2> array;
		crd3 coords;
		
		double d;
		vec3 normal;
		vec2 min, max;

		space2 tr;
	};

	inline Object_ptr makeTexturePolygon(const std::vector<vec2>& polygon, crd3 coords, Image_ptr img, const space2& tr) { return Object_ptr(new TexturePolygon(polygon, coords, img, tr)); }

};

#endif