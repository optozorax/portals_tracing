#ifndef PT_POLYGON_H
#define PT_POLYGON_H

#include <vector>
#include <pt/object.h>

namespace pt
{

	struct Point
	{
		double x, y;
	};

	class Polygon : public Shape
	{
	public:
		Polygon(std::vector<Point> polygon, Material* material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   float tMin, 
					   float tMax) const;

		std::vector<Point> array;
	};

};

#endif