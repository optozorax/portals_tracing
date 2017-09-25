#ifndef PT_360_H
#define PT_360_H

#include <pt/camera.h>

namespace pt
{

	class Camera360 : public Camera
	{
	public:
		Camera360(Vector pos, Vector lookToward, double resulution);

		Ray getRay(float x, float y);

		/** Направляет ортогональную камеру в указанном направлении. */
		void lookTowards(Vector toward);

		Vector pos;
	};

};

#endif