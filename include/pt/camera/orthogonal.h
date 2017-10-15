#ifndef PT_ORTHOGONAL_H
#define PT_ORTHOGONAL_H

#include <pt/camera.h>

namespace pt
{

	class Orthogonal : public Camera
	{
	public:
		Orthogonal(Vector pos, double scale, double width, double height);

		Ray getRay(float x, float y);

		/** Направляет ортогональную камеру в указанную точку. */
		void lookTowards(Vector toward);

		Vector i, j, k;
		double scale;
		double width;
		double height;
	};

};

#endif