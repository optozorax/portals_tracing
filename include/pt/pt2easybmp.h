#ifndef PT_PT2EASYBMP_H
#define PT_PT2EASYBMP_H

#include <string>
#include <pt/image.h>

namespace pt
{

	void saveAsBmp(Image& img, std::string name);
	void loadAsBmp(Image& img, std::string name);

};

#endif