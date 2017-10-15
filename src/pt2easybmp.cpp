#include <EasyBMP.h>
#include <pt/pt2easybmp.h>

namespace pt
{

//-----------------------------------------------------------------------------
void saveAsBmp(Image& img, std::string name) {
	BMP AnImage;
	AnImage.SetBitDepth(32);
	AnImage.SetSize(img.getWidth(), img.getHeight());

	RGBApixel pix;
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			pix.Alpha = img(i, j).a * 255;
			pix.Red = img(i, j).r * 255;
			pix.Green = img(i, j).g * 255;
			pix.Blue = img(i, j).b * 255;

			AnImage.SetPixel(i, j, pix);
		}
	}

	AnImage.WriteToFile(name);
}

//-----------------------------------------------------------------------------
void loadAsBmp(Image& img, std::string name) {
	// @TODO make this
}

};