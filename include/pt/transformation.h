#ifndef PT_TRANSFORMATION_H
#define PT_TRANSFORMATION_H

#include <pt/object.h>

namespace pt
{

	class Transformation;
	class TrMove;
	class TrRotate;
	class TrScale;
	/*class TrLookAt;*/

	//-------------------------------------------------------------------------
	class Transformation : public Object
	{
	public:
		Transformation(Object_ptr obj) : m_obj(obj) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

		virtual vec3 transform(const vec3& p) const = 0;
		virtual vec3 inverse(const vec3& p) const = 0;
		
	private:
		Object_ptr m_obj;
		mutable Ray ray1;
		mutable Intersection inter1;
	};

	//-------------------------------------------------------------------------
	class TrMove : public Transformation
	{
	public:
		TrMove(const vec3& diff, Object_ptr obj);

		vec3 transform(const vec3& p) const;
		vec3 inverse(const vec3& p) const;

		vec3 diff;
	};

	//-------------------------------------------------------------------------
	class TrRotate : public Transformation
	{
	public:
		TrRotate(const vec3& angles, Object_ptr obj);

		vec3 transform(const vec3& p) const;
		vec3 inverse(const vec3& p) const;

		void rotate2(double& x, double &y, const double& angle) const;

		vec3 angles; 
	};

	//-------------------------------------------------------------------------
	class TrScale : public Transformation
	{
	public:
		TrScale(const vec3& scale, Object_ptr obj);

		vec3 transform(const vec3& p) const;
		vec3 inverse(const vec3& p) const;

		vec3 scale;
	};

	//-------------------------------------------------------------------------
	class TrComplex : public Transformation
	{
	public:
		TrComplex(const vec3& scale, const vec3& angles, const vec3& diff, Object_ptr obj);

		vec3 transform(const vec3& p) const;
		vec3 inverse(const vec3& p) const;

		vec3& scale;
		vec3& angles;
		vec3& diff;
	private:
		TrScale m_scale;
		TrRotate m_rotate;
		TrMove m_move;
	};

};

#endif