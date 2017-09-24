#ifndef PT_OBJECT_H
#define PT_OBJECT_H

#include <pt/vector.h>
#include <pt/basics.h>

namespace pt
{

	struct Ray;
	struct Intersection;
	class Object;

	//-------------------------------------------------------------------------
	struct Ray
	{
		Vector pos;
		Vector dir;
	};

	//-------------------------------------------------------------------------
	struct Intersection
	{
		float t;
		Vector pos;
		Vector normal;
	};

	//-------------------------------------------------------------------------
	class Object
	{
	public:
		virtual ~Object() {}
		virtual bool intersect(const Ray& ray, 
							   Intersection& inter, 
							   float tMin, 
							   float tMax) = 0 const;
		virtual bool scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered) = 0 const;
	};

};

#endif