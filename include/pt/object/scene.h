#ifndef PT_SCENE_H
#define PT_SCENE_H

#include <vector>
#include <pt/object.h>
#include <prtl_vis/scene_reader.h>

namespace pt
{

	class Scene : public Object
	{
	public:
		bool intersect(const Ray& ray, 
					   Intersection& inter, 
					   double tMin, 
					   double tMax) const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const;

		void add(Object_ptr obj);

		std::vector<Object_ptr> array;
	};

	Scene loadFrame(const scene::Frame& frame);

};

#endif