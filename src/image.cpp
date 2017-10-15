#include <pt/image.h>

namespace pt
{

//-----------------------------------------------------------------------------
Image::Image(int width, int height) {
	m_pix = new Color[width * height];
	m_width = width;
	m_height = height;
}

//-----------------------------------------------------------------------------
Image::~Image() {
	delete[] m_pix;
}

//-----------------------------------------------------------------------------
void Image::resize(int width, int height) {
	delete[] m_pix;
	m_pix = new Color[width * height];
	m_width = width;
	m_height = height;
}

//-----------------------------------------------------------------------------
void Image::colorCorrection(void) {
	for (int i = 0; i < m_width * m_height; ++i) {
		m_pix[i] = m_pix[i].sqrt();
		if (m_pix[i].a > 1) m_pix[i].a = 1;
		if (m_pix[i].r > 1) m_pix[i].r = 1;
		if (m_pix[i].g > 1) m_pix[i].g = 1;
		if (m_pix[i].b > 1) m_pix[i].b = 1;
	}

}

//-----------------------------------------------------------------------------
int Image::getWidth() const {
	return m_width;
}

//-----------------------------------------------------------------------------
int Image::getHeight() const {
	return m_height;
}

//-----------------------------------------------------------------------------
Color& Image::operator()(int x, int y) {
	return m_pix[x + y * m_width];
}

//-----------------------------------------------------------------------------
const Color& Image::operator()(int x, int y) const {
	return m_pix[x + y * m_width];	
}

};