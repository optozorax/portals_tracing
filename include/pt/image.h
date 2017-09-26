#ifndef PT_IMAGE_H
#define PT_IMAGE_H

#include <pt/basics.h>

namespace pt
{

	class Image
	{
	public:
		Image(int width, int height);
		~Image();

		void resize(int width, int height);

		/** Берет из цветов квадратный корень, потому что монитор представляет цвет пикселя не линейно возрастающий, а нелинейно, и так как этот рисунок - результат рендеринга, то здесь так же надо представлять цвета мира, чтобы они корректно отображались на экране. */
		void colorCorrection(void);

		int getWidth() const;
		int getHeight() const;
		Color& operator()(int x, int y);
		const Color& operator()(int x, int y) const;
	private:
		Color*	m_pix;
		int 	m_width;
		int		m_height;
	};

};

#endif