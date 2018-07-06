#ifndef PT_CAMERA_H
#define PT_CAMERA_H

#include <pt/basics.h>
#include <pt/object.h>

namespace pt
{

	vec3 getRotatedVector(const vec3& pos, double r, double alpha, double beta);

	//-------------------------------------------------------------------------
	class Camera
	{
	public:
		Camera(vec3 pos) : pos(pos) {}

		virtual ~Camera() {}
		virtual Ray getRay(double x, double y, bool isDiffuse) const = 0;

		vec3 pos;
	};

	//-------------------------------------------------------------------------
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(double focal, 
						  double viewAngle, 
						  double aperture, 
						  vec3 pos, 
						  double width, 
						  double height);

		void assign(double focal1, 
					double viewAngle1, 
					double aperture1, 
					vec3 pos1, 
					double width1, 
					double height1);

		Ray getRay(double x, double y, bool isDiffuse) const;

		void lookAt(const vec3& towards);

		double focal;
		double aperture;
		double viewAngle;
		vec3 i, j, k;
		double h;
		double width, height;
	};

};

#endif