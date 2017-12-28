#ifndef PT_BASICS_H
#define PT_BASICS_H

#include <vector>
#include <pt/vector.h>

namespace pt
{

	class Color;
	struct CoordSystem;
	struct Vector2;

	//-------------------------------------------------------------------------
	const double pi = 3.141592653589793238462643383279;
	double random(void);

	//-------------------------------------------------------------------------
	class Color
	{
	public:
		double r, g, b, a;
		Color() : r(0), g(0), b(0), a(0) {}
		Color(double r, double g, double b, double a = 1) : r(r), g(g), b(b), a(a) {}

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
	struct CoordSystem
	{
		Vector i, j, k;
		Vector pos;
	};

	const CoordSystem standard = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {0, 0, 0}};

	/** Выражает вектор p из обычных координат в координаты new. При этом его абсолютное расположение сохраняется.  */
	Vector toCoordSystem(const CoordSystem& newsys, Vector p);

	/** Вектор p задан в координатах current, преобразует его к абсолютным координатам и возвращает. Его абсолютное положение сохраняется. */
	Vector fromCoordSystem(const CoordSystem& current, const Vector& p);

	/** Предполагает, что точка absolutePos(заданная в абсоютных координатах) вошла в портал, заданный системой координат first. Телепортирует его в систему координат second, при этом возвращая абсолютные координаты. Относительное расположение результирующего вектора в системе координат second точно такое же, как и в системе координат first. */
	Vector teleportVector(const CoordSystem& first,
						  const CoordSystem& second,
						  const Vector& absolutePos);

	/** Телепортирует направление. Направление всегда находится началом в начале абсоютный координат. */
	Vector teleportDirection(const CoordSystem& first,
							 const CoordSystem& second,
							 const Vector& direction);

	/** Аналогично функции teleportVector телепортирует систему координат, сохраняя относительное положение результата относительно second таким же, как и относительное положение p относительно second. */
	CoordSystem teleportCoordSystem(const CoordSystem& first,
									const CoordSystem& second,
									const CoordSystem& p);

	/** Вектор a - начало этой системы координат. Вектор (b-a) - ось I. Ортогональная часть из (c-a) - ось J. Векторное произведение I на J - ось K. Обязательное условие - три точки с заданными радиус-векторами не должны находиться на одной прямой!!! */
	CoordSystem calculateCoordSystem(const Vector& a, 
									 const Vector& b, 
									 const Vector& c);

	/** Определяет, является ли система координат правой. */
	bool isRightCoordSystem(const CoordSystem& coords);

	/** Поворачивает систему координат по заданным углам эйлера angles. См. https://ru.wikipedia.org/wiki/Углы_Эйлера . */
	CoordSystem rotate(const CoordSystem& coords, Vector angles);

	//-------------------------------------------------------------------------
	struct Vector2 
	{
		double x, y;
	};

	typedef std::vector<Vector2> Poly2;

	bool pointInPolygon(const Poly2& poly, Vector2 p);

};

#endif