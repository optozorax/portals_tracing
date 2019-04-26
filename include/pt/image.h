#ifndef PT_IMAGE_H
#define PT_IMAGE_H

#include <memory>
#include <string>
#include <pt/basics.h>

namespace pt
{

	class Image
	{
	public:
		Image(int width, int height);
		Image(const Image& img);
		Image(const Image* img);
		~Image();

		void resize(int width, int height);

		/** Берет из цветов квадратный корень, потому что монитор представляет цвет пикселя не линейно возрастающий, а нелинейно, и так как этот рисунок - результат рендеринга, то здесь так же надо представлять цвета мира, чтобы они корректно отображались на экране. */
		void colorCorrection(void);

		void clear(void);

		int getWidth() const;
		int getHeight() const;
		Color& operator()(int x, int y);
		const Color& operator()(int x, int y) const;
	private:
		Color*	m_pix;
		int 	m_width;
		int		m_height;
	};

	typedef std::shared_ptr<Image> Image_ptr;

	//-------------------------------------------------------------------------
	void saveAsDoubleImg(const Image& img, const std::string& name);
	void loadAsDoubleImg(Image& img, const std::string& name);

	void toGrayScaleDoubleImg(Image& img);

};

#endif