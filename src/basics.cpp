#include <math.h>
#include <pt/basics.h>

namespace pt
{

//-----------------------------------------------------------------------------
float random(void) {
	// @TODO make this
}

//-----------------------------------------------------------------------------
Vector randomSphere(void) {
	// @TODO make this
}

//-----------------------------------------------------------------------------
Color& Color::operator/=(double k) {
	r /= k;
	g /= k;
	b /= k;
	a /= k;
}

//-----------------------------------------------------------------------------
Color& Color::operator*(const Color& a) {
	// @TODO test this
	Color clr = *this;
	clr.r = clr.r + a.r*(1 - clr.a);
	clr.g = clr.g + a.g*(1 - clr.a);
	clr.b = clr.b + a.b*(1 - clr.a);
	clr.a = clr.a + a.a*(1 - clr.a);
	return clr;
}

//-----------------------------------------------------------------------------
Color& Color::sqrt(void) {
	r = ::sqrt(r);
	g = ::sqrt(g);
	b = ::sqrt(b);
}

};