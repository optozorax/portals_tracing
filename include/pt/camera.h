#ifndef PT_CAMERA_H
#define PT_CAMERA_H

#include <pt/Vector.h>
#include <pt/object.h>

namespace pt
{

	Vector getRotatedVector(const Vector& pos, double r, double alpha, double beta);

	//-------------------------------------------------------------------------
	class Camera
	{
	public:
		Camera(Vector pos) : pos(pos) {}

		virtual ~Camera() {}
		virtual Ray getRay(double x, double y) const = 0;

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

		void assign(double focal1, 
					double viewAngle1, 
					double aperture1, 
					Vector pos1, 
					double width1, 
					double height1);

		Ray getRay(double x, double y) const;

		void lookAt(const Vector& towards);

		double focal;
		double aperture;
		double viewAngle;
		Vector i, j, k;
		double h;
		double width, height;
	};

};

#endif