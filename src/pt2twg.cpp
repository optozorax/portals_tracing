#include <pt/pt2twg.h>

namespace pt
{

//-----------------------------------------------------------------------------
void toTwgImage(const Image& img, twg::ImageBase& twgImg) {
	using namespace twg;
	twgImg.resize(Point_i(img.getWidth(), img.getHeight()));
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			twgImg[Point_i(i, j)] = twg::rgba(img(i, j).r * 255, img(i, j).g * 255, img(i, j).b * 255, img(i, j).a * 255);
		}
	}
}

//-----------------------------------------------------------------------------
void toPtImage(Image& img, const twg::ImageBase& twgImg) {
	using namespace twg;
	img.resize(twgImg.width(), twgImg.height());
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			ColorUnion pix(twgImg[Point_i(i, j)]);
			img(i, j).a = pix.rgba.a / 255.0;
			img(i, j).r = pix.rgba.r / 255.0;
			img(i, j).g = pix.rgba.g / 255.0;
			img(i, j).b = pix.rgba.b / 255.0;
		}
	}
}

};