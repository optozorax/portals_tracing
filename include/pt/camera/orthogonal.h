#ifndef PT_ORTHOGONAL_H
#define PT_ORTHOGONAL_H

#include <pt/camera.h>

namespace pt
{

	class Orthogonal : public Camera
	{
	public:
		Orthogonal(Vector pos, Vector lookToward, double resulution);

		Ray getRay(float x, float y);

		/** Направляет ортогональную камеру в указанном направлении. */
		void lookTowards(Vector toward);

		Vector pos;
	};

};

#endif