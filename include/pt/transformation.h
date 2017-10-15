#ifndef PT_TRANSFORMATION_H
#define PT_TRANSFORMATION_H

#include <pt/object.h>

namespace pt
{

	class Transformation;
	/*class TrMove;
	class TrRotate;
	class TrScale;
	class TrLookAt;*/

	//-------------------------------------------------------------------------
	class Transformation : public Object
	{
	public:
		Transformation(Object* obj) : m_obj(obj) {}
		virtual ~Transformation() {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

	protected:
		virtual Ray transform(const Ray& ray) const = 0;
		virtual Ray inverse(const Ray& ray) const = 0;
		
	private:
		Object* m_obj;
	};

};

#endif