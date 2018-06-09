#pragma once

namespace pt
{

	//-------------------------------------------------------------------------
	class Vector2
	{
	public:
		double x, y;

		Vector2();
		Vector2(double a);
		Vector2(double x, double y);

		Vector2& negate(void);
		Vector2& normalize(void);

		double getLength(void) const;
		double getLengthSqr(void) const;

		//---------------------------------------------------------------------
		Vector2& operator+=(const Vector2& a);
		Vector2& operator-=(const Vector2& a);
		Vector2& operator*=(double a);
		Vector2& operator/=(double a);
	};

	//-------------------------------------------------------------------------
	Vector2 operator-(const Vector2& a);
	Vector2 operator+(const Vector2& a, const Vector2& b);
	Vector2 operator-(const Vector2& a, const Vector2& b);
	Vector2 operator*(const Vector2& a, double k);
	Vector2 operator/(const Vector2& a, double k);

	bool operator==(const Vector2& a, const Vector2& b);
	bool operator!=(const Vector2& a, const Vector2& b);

	//-------------------------------------------------------------------------
	double dot(const Vector2& a, const Vector2& b);

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline Vector2::Vector2() : Vector2(0) {}

//-----------------------------------------------------------------------------
inline Vector2::Vector2(double a) : x(a), y(a) {}

//-----------------------------------------------------------------------------
inline Vector2::Vector2(double x, double y) : x(x), y(y) {}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::negate(void) {
	x = -x;
	y = -y;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::normalize(void) {
	double length = getLength();
	x /= length;
	y /= length;
	return *this;
}

//-----------------------------------------------------------------------------
inline double Vector2::getLength(void) const {
	return sqrt(x*x + y*y);
}

//-----------------------------------------------------------------------------
inline double Vector2::getLengthSqr(void) const {
	return x*x + y*y;
}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::operator+=(const Vector2& a) {
	x += a.x;
	y += a.y;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::operator-=(const Vector2& a) {
	x -= a.x;
	y -= a.y;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::operator*=(double a) {
	x *= a;
	y *= a;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector2& Vector2::operator/=(double a) {
	x /= a;
	y /= a;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector2 operator-(const Vector2& a) {
	return Vector2(-a.x, -a.y);
}

//-----------------------------------------------------------------------------
inline Vector2 operator+(const Vector2& a, const Vector2& b) {
	return Vector2(a.x + b.x, a.y + b.y);
}

//-----------------------------------------------------------------------------
inline Vector2 operator-(const Vector2& a, const Vector2& b) {
	return Vector2(a.x - b.x, a.y - b.y);
}

//-----------------------------------------------------------------------------
inline Vector2 operator*(const Vector2& a, double k) {
	return Vector2(a.x * k, a.y * k);
}

//-----------------------------------------------------------------------------
inline Vector2 operator/(const Vector2& a, double k) {
	return Vector2(a.x / k, a.y / k);
}

//-----------------------------------------------------------------------------
inline bool operator==(const Vector2& a, const Vector2& b) {
	return a.x == b.x && a.y == b.y;
}

//-----------------------------------------------------------------------------
inline bool operator!=(const Vector2& a, const Vector2& b) {
	return a.x != b.x && a.y != b.y;
}

//-----------------------------------------------------------------------------
inline double dot(const Vector2& a, const Vector2& b) {
	return a.x * b.x + a.y * b.y;
}

};