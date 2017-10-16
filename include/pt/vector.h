#ifndef PT_VECTOR_H
#define PT_VECTOR_H

#include <math.h>

namespace pt
{

	//-------------------------------------------------------------------------
	class Vector
	{
	public:
		double x, y, z;

		Vector();
		Vector(double a);
		Vector(double x, double y, double z);

		Vector& negate(void);
		Vector& normalize(void);

		double getLength(void) const;
		double getLengthSqr(void) const;

		//---------------------------------------------------------------------
		Vector& operator+=(const Vector& a);
		Vector& operator-=(const Vector& a);
		Vector& operator*=(double a);
		Vector& operator/=(double a);
	};

	//-------------------------------------------------------------------------
	Vector operator-(const Vector& a);
	Vector operator+(const Vector& a, const Vector& b);
	Vector operator-(const Vector& a, const Vector& b);
	Vector operator*(const Vector& a, double k);
	Vector operator/(const Vector& a, double k);

	bool operator==(const Vector& a, const Vector& b);
	bool operator!=(const Vector& a, const Vector& b);

	//-------------------------------------------------------------------------
	Vector cross(const Vector& a, const Vector& b);
	double dot(const Vector& a, const Vector& b);
	void reflect(Vector& ray, const Vector& normal);
	bool refract(Vector& ray, const Vector& normal, double refractiveIndex);
	Vector randomSphere(void);

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline Vector::Vector() : Vector(0) {}

//-----------------------------------------------------------------------------
inline Vector::Vector(double a) : x(a), y(a), z(a) {}

//-----------------------------------------------------------------------------
inline Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

//-----------------------------------------------------------------------------
inline Vector& Vector::negate(void) {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector& Vector::normalize(void) {
	double length = getLength();
	x /= length;
	y /= length;
	z /= length;
	return *this;
}

//-----------------------------------------------------------------------------
inline double Vector::getLength(void) const {
	return sqrt(x*x + y*y + z*z);
}

//-----------------------------------------------------------------------------
inline double Vector::getLengthSqr(void) const {
	return x*x + y*y + z*z;
}

//-----------------------------------------------------------------------------
inline Vector& Vector::operator+=(const Vector& a) {
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector& Vector::operator-=(const Vector& a) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector& Vector::operator*=(double a) {
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector& Vector::operator/=(double a) {
	x /= a;
	y /= a;
	z /= a;
	return *this;
}

//-----------------------------------------------------------------------------
inline Vector operator-(const Vector& a) {
	return Vector(-a.x, -a.y, -a.z);
}

//-----------------------------------------------------------------------------
inline Vector operator+(const Vector& a, const Vector& b) {
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

//-----------------------------------------------------------------------------
inline Vector operator-(const Vector& a, const Vector& b) {
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

//-----------------------------------------------------------------------------
inline Vector operator*(const Vector& a, double k) {
	return Vector(a.x * k, a.y * k, a.z * k);
}

//-----------------------------------------------------------------------------
inline Vector operator/(const Vector& a, double k) {
	return Vector(a.x / k, a.y / k, a.z / k);
}

//-----------------------------------------------------------------------------
inline bool operator==(const Vector& a, const Vector& b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

//-----------------------------------------------------------------------------
inline bool operator!=(const Vector& a, const Vector& b) {
	return a.x != b.x && a.y != b.y && a.z != b.z;
}

};

#endif