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
		PerspectiveCamera(double focal, 
						  double viewAngle, 
						  double aperture, 
						  Vector pos, 
						  double width, 
						  double height);

		Ray getRay(float x, float y);

		void lookAt(const Vector& towards);

		double focal;
		double aperture;
		double viewAngle;
		Vector toward;
		Vector i, j, k;
		double h;
		double width, height;
	};

};

#endif