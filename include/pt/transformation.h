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
					   float tMin, 
					   float tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

	protected:
		virtual Ray transform(const Ray& ray) = 0 const;
		virtual Ray iverse(const Ray& ray) = 0 const;
		
	private:
		Object* obj;
	};

};

#endif