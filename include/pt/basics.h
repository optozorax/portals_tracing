#ifndef PT_BASICS_H
#define PT_BASICS_H

#include <prtl/coord_system.h>
#include <prtl/vector2.h>
#include <prtl/vector3.h>
#include <prtl/common.h>
#include <prtl/polygon.h>

namespace pt
{

	// Используем это сущности из prtl
	using prtl::vec3;
	using prtl::vec2;
	using prtl::CoordSystem;
	using prtl::pointInPolygon;
	using prtl::dot;
	using prtl::cross;
	using prtl::calculateCoordSystem;
	using prtl::computeCenter;
	using prtl::computeContour;
	using prtl::fromCoordSystem;
	using prtl::fromCoordSystemDirection;
	using prtl::isCoordSystemNear;
	using prtl::isPolygonsEqual;
	using prtl::isRightCoordSystem;
	using prtl::isVec2Near;
	using prtl::isVec3Near;
	using prtl::operator*;
	using prtl::operator+;
	using prtl::operator-;
	using prtl::operator/;
	using prtl::rotate;
	using prtl::rotateAroundAxis;
	using prtl::standard;
	using prtl::teleportCoordSystem;
	using prtl::teleportDirection;
	using prtl::teleportVector;
	using prtl::toCoordSystem;
	using prtl::toCoordSystemDirection;
	using prtl::transform;

	//-------------------------------------------------------------------------
	const double pi = 3.141592653589793238462643383279;
	double random(void);

	//-------------------------------------------------------------------------
	class Color
	{
	public:
		double r, g, b, a;
		Color() : r(0), g(0), b(0), a(0) {}
		Color(double r, double g, double b, double a = 1) : r(r*r), g(g*g), b(b*b), a(a) {}

		/** Делит цвет на некоторое число для взятия среднего арифметического среди нескольких цветов. Среднее арифметическое так же берется и для альфа канала. */
		Color operator/=(double k);

		/** Умножает два цвета, только в пространстве r, g, b. Если один из цветов Transparent, то возвращает текущий цвет.

		ИЛИ. Считает, что оба цвета полупрозрачные и считает как если бы текущий накладывался поверх a. */
		Color operator*(const Color& a);

		Color operator*(double a);
		Color operator+=(const Color& a);

		/** Получает квадратный корень из цвета. При этом квадратный корень из альфа-канала не берется. */
		Color sqrt(void);
	};

	const Color transparent(0, 0, 0, 0);

	Color overlay(const Color& upper, const Color& lower);

	//-------------------------------------------------------------------------
	void reflect(vec3& ray, const vec3& normal);
	bool refract(vec3& ray, const vec3& normal, double refractiveIndex);
	vec3 randomSphere(void);

};

#endif