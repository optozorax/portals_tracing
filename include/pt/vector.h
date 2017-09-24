#ifndef PT_VECTOR_H
#define PT_VECTOR_H

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
	Vector operator-(const Vector& a) const;
	Vector operator+(const Vector& a, const Vector& b) const;
	Vector operator-(const Vector& a, const Vector& b) const;
	Vector operator*(const Vector& a, double k) const;
	Vector operator/(const Vector& a, double k) const;

	bool operator==(const Vector& a, const Vector& b);
	bool operator!=(const Vector& a, const Vector& b);

	//-------------------------------------------------------------------------
	Vector cross(const Vector& a, const Vector& b);
	double dot(const Vector& a, const Vector& b);
	void reflect(Vector& ray, const Vector& normal);
	bool refract(Vector& ray, const Vector& normal, double refractiveIndex);

};

#endif