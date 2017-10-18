#include <random>
#include <math.h>
#include <pt/basics.h>

namespace pt
{

std::mt19937 generator;
std::uniform_real_distribution<double> distribution(0, 1);

//-----------------------------------------------------------------------------
double random(void) {
	return distribution(generator);
}

//-----------------------------------------------------------------------------
Color& Color::operator/=(double k) {
	r /= k;
	g /= k;
	b /= k;
	a /= k;
	return *this;
}

//-----------------------------------------------------------------------------
Color& Color::operator*(const Color& a) {
	// @TODO test this
	Color clr = *this;
	//clr.r = clr.r + a.r*(1 - clr.a);
	//clr.g = clr.g + a.g*(1 - clr.a);
	//clr.b = clr.b + a.b*(1 - clr.a);
	clr.a = clr.a + a.a*(1 - clr.a);
	clr.r *= a.r;
	clr.g *= a.g;
	clr.b *= a.b;
	return clr;
}

//-----------------------------------------------------------------------------
Color& Color::operator+=(const Color& clr) {
	a += clr.a;
	r += clr.r;
	g += clr.g;
	b += clr.b;
	return *this;
}

//-----------------------------------------------------------------------------
Color& Color::operator*(double a) {
	Color clr = *this;
	clr.r *= a;
	clr.g *= a;
	clr.b *= a;
	return clr;
}

//-----------------------------------------------------------------------------
Color& Color::sqrt(void) {
	r = ::sqrt(r);
	g = ::sqrt(g);
	b = ::sqrt(b);
	return *this;
}

};