//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include <stb_image.h>
#include <stb_image_write.h>

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

//-----------------------------------------------------------------------------
void saveAsPng(Image& img, std::string name) {
	unsigned char *data = new unsigned char[img.getWidth() * img.getHeight() * 4];
	for (int i = 0; i < img.getHeight(); i++) {
		for (int j = 0; j < img.getWidth(); j++) {
			int offset = 4*(i * img.getWidth() + j);
			auto clr = img(j, i);
			data[offset + 0] = clr.r * 255;
			data[offset + 1] = clr.g * 255;
			data[offset + 2] = clr.b * 255;
			data[offset + 3] = clr.a * 255;
		}
	}
	stbi_write_png_compression_level = 64;
	stbi_write_png(name.c_str(), img.getWidth(), img.getHeight(), 4, data, img.getWidth() * 4);
	delete[] data;
}

//-----------------------------------------------------------------------------
void loadAsPng(Image& img, std::string name) {
	unsigned char *data;
	int width, height, n;
	data = stbi_load(name.c_str(), &width, &height, &n, 4);
	img.resize(width, height);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			img(j, i) = Color(
				data[4*(i * width + j)+0]/255.0,
				data[4*(i * width + j)+1]/255.0, 
				data[4*(i * width + j)+2]/255.0,
				data[4*(i * width + j)+3]/255.0
			);
		}
	}
	free(data);
}


};