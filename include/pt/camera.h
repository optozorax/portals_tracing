#ifndef PT_CAMERA_H
#define PT_CAMERA_H

#include <pt/Vector.h>
#include <pt/object.h>

namespace pt
{

	class Camera
	{
	public:
		Camera(Vector pos) : pos(pos) {}

		virtual ~Camera() {}
		virtual Ray getRay(float x, float y) = 0;

		Vector pos;
	};

};

#endif