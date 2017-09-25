#ifndef PT_TRANSFORMATION_H
#define PT_TRANSFORMATION_H

#include <pt/object.h>

namespace pt
{

	class Transformation : public Object
	{
	public:
		Transformation(Object* obj) : m_obj(obj) {}
		virtual ~Transformation() {}

	protected:
		virtual Ray transform(const Ray& ray) = 0;
		virtual Ray iverse(const Ray& ray) = 0;
		
	private:
		Object* obj;
	};

};

#endif