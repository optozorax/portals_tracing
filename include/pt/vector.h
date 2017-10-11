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
	Vector randomVector(void);

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
Vector::Vector(double a) : x(a), y(a), z(a) {}

//-----------------------------------------------------------------------------
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

//-----------------------------------------------------------------------------
Vector& Vector::negate(void) {

}

//-----------------------------------------------------------------------------
Vector& Vector::normalize(void) {
	
}

//-----------------------------------------------------------------------------
double Vector::getLength(void) {
	
}

//-----------------------------------------------------------------------------
double Vector::getLengthSqr(void) {
	
}

//-----------------------------------------------------------------------------
Vector& Vector::operator+=(const Vector& a) {
	
}

//-----------------------------------------------------------------------------
Vector& Vector::operator-=(const Vector& a) {
	
}

//-----------------------------------------------------------------------------
Vector& Vector::operator*=(double a) {
	
}

//-----------------------------------------------------------------------------
Vector& Vector::operator/=(double a) {
	
}

//-----------------------------------------------------------------------------
Vector operator-(const Vector& a) {

}

//-----------------------------------------------------------------------------
Vector operator+(const Vector& a, const Vector& b) {

}

//-----------------------------------------------------------------------------
Vector operator-(const Vector& a, const Vector& b) {

}

//-----------------------------------------------------------------------------
Vector operator*(const Vector& a, double k) {

}

//-----------------------------------------------------------------------------
Vector operator/(const Vector& a, double k) {

}

//-----------------------------------------------------------------------------
bool operator==(const Vector& a, const Vector& b) {

}

//-----------------------------------------------------------------------------
bool operator!=(const Vector& a, const Vector& b) {
	
}

};

#endif