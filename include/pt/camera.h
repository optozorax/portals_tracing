#ifndef PT_CAMERA_H
#define PT_CAMERA_H

#include <pt/Vector.h>
#include <pt/object.h>

namespace pt
{

	//-------------------------------------------------------------------------
	class Camera
	{
	public:
		Camera(Vector pos) : pos(pos) {}

		virtual ~Camera() {}
		virtual Ray getRay(float x, float y) = 0;

		Vector pos;
	};

	//-------------------------------------------------------------------------
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(double focal, double viewAngle, Vector pos);

		Ray getRay(float x, float y);

		void lookAt(const Vector& towards);

		double focal;
		double viewAngle;
		Vector toward;
	};

};

#endif