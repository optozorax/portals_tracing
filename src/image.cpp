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
Image::Image(const Image& img) {
	m_pix = new Color[img.m_width * img.m_height];
	m_width = img.m_width;
	m_height = img.m_height;
	for (int i = 0; i < m_width * m_height; ++i) {
		m_pix[i] = img.m_pix[i];
	}
}

//-----------------------------------------------------------------------------
Image::Image(const Image * img) {
	try {
		m_pix = new Color[img->m_width * img->m_height];
		m_width = img->m_width;
		m_height = img->m_height;
		for (int i = 0; i < m_width * m_height; ++i) {
			m_pix[i] = img->m_pix[i];
		}
	} catch(...) {
		m_pix = new Color[1000 * 1000];
		m_width = 1000;
		m_height = 1000;
		for (int i = 0; i < m_width * m_height; ++i) {
			m_pix[i] = Color(0, 0, 0);
		}
	}
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
void Image::clear(void) {
	for (int i = 0; i < m_width * m_height; ++i) {
		m_pix[i] = Color(0, 0, 0, 0);
	}
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