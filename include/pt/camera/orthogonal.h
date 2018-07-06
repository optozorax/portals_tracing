#ifndef PT_ORTHOGONAL_H
#define PT_ORTHOGONAL_H

#include <pt/camera.h>

namespace pt
{

	class Orthogonal : public Camera
	{
	public:
		Orthogonal(vec3 pos, double scale, double width, double height);

		Ray getRay(double x, double y, bool isDiffuse) const;

		/** Направляет ортогональную камеру в указанную точку. */
		void lookTowards(vec3 toward);

		double scale;
		double width;
		double height;
	private:
		vec3 i, j, k;
	};

};

#endif