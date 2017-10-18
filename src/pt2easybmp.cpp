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

	std::wstring wstr(name.begin(), name.end());
	AnImage.WriteToFile(wstr.c_str());
}

//-----------------------------------------------------------------------------
void loadAsBmp(Image& img, std::string name) {
	//@ todo test this
	BMP AnImage;
	std::wstring wstr(name.begin(), name.end());
	AnImage.WriteToFile(wstr.c_str());

	RGBApixel pix;
	for (int i = 0; i < AnImage.TellWidth(); i++) {
		for (int j = 0; j < AnImage.TellHeight(); j++) {
			pix = AnImage.GetPixel(i, j);
			img(i, j).a = pix.Alpha / 255.0;
			img(i, j).r = pix.Red / 255.0;
			img(i, j).g = pix.Green / 255.0;
			img(i, j).b = pix.Blue / 255.0;
		}
	}
}

};