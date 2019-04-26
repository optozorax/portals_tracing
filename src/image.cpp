#include <fstream>
#include <algorithm>
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void saveAsDoubleImg(const Image& img, const std::string& name) {
	std::ofstream fout(name, std::ios::binary);
	double value = 0;
	value = img.getWidth(); fout.write(reinterpret_cast<const char*>(&value), sizeof(double));
	value = img.getHeight(); fout.write(reinterpret_cast<const char*>(&value), sizeof(double));
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			value = img(i, j).r; fout.write(reinterpret_cast<const char*>(&value), sizeof(double));
		}
	}
	fout.close();
}

//-----------------------------------------------------------------------------
void loadAsDoubleImg(Image& img, const std::string& name) {
	std::ifstream fin(name, std::ios::binary);
	double value = 0, width, height;
	fin.read(reinterpret_cast<char*>(&value), sizeof(double)); width = value;
	fin.read(reinterpret_cast<char*>(&value), sizeof(double)); height = value;
	img.resize(width, height);
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			fin.read(reinterpret_cast<char*>(&value), sizeof(double)); img(i, j).r = value;
			img(i, j).g = 0;
			img(i, j).b = 0;
			img(i, j).a = 1;
		}
	}
	fin.close();
}

//-----------------------------------------------------------------------------
void toGrayScaleDoubleImg(Image& img) {
	double mymin = img(0, 0).r, mymax = img(0, 0).r;

	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			mymin = std::min(mymin, img(i, j).r);
			mymax = std::max(mymax, img(i, j).r);
		}
	}

	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			double value = 1 - (img(i, j).r - mymin)/(mymax-mymin);
			img(i, j).r = value;
			img(i, j).g = value;
			img(i, j).b = value;
			img(i, j).a = 1;
		}
	}
}

};