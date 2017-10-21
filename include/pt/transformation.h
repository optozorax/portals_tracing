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
		Transformation(Object* obj) : m_obj(obj) {}

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

		virtual Vector transform(const Vector& p) const = 0;
		virtual Vector inverse(const Vector& p) const = 0;
		
	private:
		Object* m_obj;
		mutable Ray ray1;
		mutable Intersection inter1;
	};

	//-------------------------------------------------------------------------
	class TrMove : public Transformation
	{
	public:
		TrMove(const Vector& diff, Object* obj);

		Vector transform(const Vector& p) const;
		Vector inverse(const Vector& p) const;

		Vector diff;
	};

	//-------------------------------------------------------------------------
	class TrRotate : public Transformation
	{
	public:
		TrRotate(const Vector& angles, Object* obj);

		Vector transform(const Vector& p) const;
		Vector inverse(const Vector& p) const;

		void rotate2(double& x, double &y, const double& angle) const;

		Vector angles; 
	};

	//-------------------------------------------------------------------------
	class TrScale : public Transformation
	{
	public:
		TrScale(const Vector& scale, Object* obj);

		Vector transform(const Vector& p) const;
		Vector inverse(const Vector& p) const;

		Vector scale;
	};

	//-------------------------------------------------------------------------
	class TrComplex : public Transformation
	{
	public:
		TrComplex(const Vector& scale, const Vector& angles, const Vector& diff, Object* obj);

		Vector transform(const Vector& p) const;
		Vector inverse(const Vector& p) const;

		Vector& scale;
		Vector& angles;
		Vector& diff;
	private:
		TrScale m_scale;
		TrRotate m_rotate;
		TrMove m_move;
	};

};

#endif