#ifndef PT_RENDERER_H
#define PT_RENDERER_H

#include <pt/camera.h>
#include <pt/image.h>
#include <pt/object.h>

namespace pt
{

	class Renderer
	{
	public:
		virtual ~Renderer() {}
		virtual void render(Camera& camera, Image& img, Object& scene) = 0 const;
	};

};

#endif