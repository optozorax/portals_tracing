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
		Polygon(std::vector<Point> polygon, Coords coords, Material* material);

		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;

		std::vector<Point> array;
		Coords coords;
	private:
		double d;
		Vector normal;
		Point min, max;
	};

};

#endif