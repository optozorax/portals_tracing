#ifndef PT_BASICS_H
#define PT_BASICS_H

#include <memory>

#include <spob/spob.h>
#include <pt/poly.h>

namespace pt
{

	// Используем это сущности из spob
	using namespace spob;

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
		Color operator*(const Color& a) const;

		Color operator*(double a) const;
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