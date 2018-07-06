#ifndef PT_ORTHOGONAL_H
#define PT_ORTHOGONAL_H

#include <pt/camera.h>

namespace pt
{

	class Orthogonal : public Camera
	{
	public:
		Orthogonal(Vector pos, double scale, double width, double height);

		Ray getRay(double x, double y, bool isDiffuse) const;

		/** Направляет ортогональную камеру в указанную точку. */
		void lookTowards(Vector toward);

		double scale;
		double width;
		double height;
	private:
		Vector i, j, k;
	};

};

#endif