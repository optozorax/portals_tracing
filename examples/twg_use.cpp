/** Рисуется вращающийся куб при помощи рейтресинга и одного источника света. Далее эта картинка переводится в картинку twg и рисуется на окне WINDOW_IMAGE32. При этом окно закрывается на esc, его можно двигать мышкой. */

#include <twg/twg.h>
#include <pt/pt.h>
#include <pt/pt2twg.h>

int main() {
	using namespace twg;
	using namespace pt;

	WindowType type(stdIcon,
		Point_i(100, 100),
		Point_i(300, 300),
		stdMin, stdMax,
		L"Rotated sphere",
		WINDOW_IMAGE32);
	WindowBase wnd(type);

	ImageBase img1(500, 250);
	Image img2(500, 250);
	PerspectiveCamera cam(1, pi/2, Vector(3, 1, 3));
	cam.lookAt(Vector(0));

	RayRenderer ren;
	ren.skyColor = Vector(0.5, 0.5, 0.5);
	ren.luminaries.push_back(PointLightSource(Vector(3, 3, 3), Color(1, 1, 1)));

	Sphere sphere(Vector(0), 1, new Scatter(Color(0.9, 0, 0.9)));
	int32 i = 0;
	while (!wnd.isClosed()) {
		i++;
		ren.render(cam, img2, sphere);
		toTwgImage(img2, img1);
		wnd.updateImage(img1);
	}
}