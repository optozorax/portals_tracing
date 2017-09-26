#ifndef PT_PT2TWG_H
#define PT_PT2TWG_H

#include <twg/image.h>
#include <pt/image.h>

namespace pt
{

	void toTwgImage(const Image& img, twg::ImageBase& twgImg);
	void toPtImage(Image& img, const twg::ImageBase& twgImg);

};

#endif