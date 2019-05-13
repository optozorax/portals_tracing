#ifndef PT_PT2EASYBMP_H
#define PT_PT2EASYBMP_H

#include <string>
#include <pt/image.h>

namespace pt
{

	void saveAsPng(Image& img, std::string name);
	void loadAsPng(Image& img, std::string name);

};

#endif