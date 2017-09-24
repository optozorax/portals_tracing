#ifndef PT_IMAGE_H
#define PT_IMAGE_H

namespace pt
{

	class Image
	{
	public:
		Image(int width, int height);
		~Image();

		void colorCorrection(void);

		void writeBmp(std::wstring name) const;
	};

};

#endif