#ifndef PT_360_H
#define PT_360_H

#include <pt/camera.h>

namespace pt
{

	class Camera360 : public Camera
	{
	public:
		Camera360(vec3 pos, double resolution);

		Ray getRay(double x, double y, bool isDiffuse) const;

		double resolution;
	};

};

#endif