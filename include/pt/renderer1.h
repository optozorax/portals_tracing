#ifndef PT_RENDERER1_H
#define PT_RENDERER1_H

#include <vector>

#include <pt/basics.h>
#include <pt/vector.h>
#include <pt/camera.h>
#include <pt/image.h>
#include <pt/object.h>
#include <pt/shape/portals.h>

namespace pt
{

	//-------------------------------------------------------------------------
	/** Точечный источник света, характеризуется положением и цветом. Так как все цвета нормализуются, то от цвета берется квадратный корень, чтобы освещение на самом деле вело себя как освещение. */
	struct PointLight
	{
		PointLight(Vector pos, Color clr) : pos(pos), clr(clr.sqrt()) {}

		Vector pos;
		Color clr;
	};

	/** Считает все возможное освещение от точечных источников освещения в данной позиции сцены, при этом учитывается наличие порталов, а так же то, что объекты могут быть полупрозрачны. */
	Color computeLight(Vector pos, Vector normal,
					   const Object& scene,
					   const std::vector<PointLight>& luminaries,
					   const std::vector<Portals*>& portals,
					   std::vector<std::pair<Portals, Vector> >& teleportation,
					   int depth);

	//-------------------------------------------------------------------------
	/** Абстрактный класс рендерера. */
	class Renderer1
	{
	public:
		Renderer1(const Camera& camera, 
				 const Object& scene,
				 Image& img) : camera(camera), scene(scene), img(img) {}
		virtual ~Renderer1() {}

		virtual void render(void) = 0;
	protected:
		const Camera&	camera;
		const Object&	scene;
		Image&			img;

		virtual void onStartRender(void) {}
		virtual void onEveryLine(double percent) {}
		virtual void onEndRendering(void) {}

		virtual Color computePixel(int x, int y) = 0;
		virtual Color computeColor(Ray ray) = 0;
	};

	//-------------------------------------------------------------------------
	/** Стандартный класс рендеринга, на основе которого строятся ray- и path-tracing. Имеет в себе не только простой рендеринг, но еще и фичу: точечные источники освещения, которые поддерживают не только полупрозрачные объекты, но еще и порталы. Вообще освещение можно было бы моделировать при помощи сфер с материалом light, но это будет слишком долго. */
	class StandardRendererWithPointLight : public Renderer1
	{
	public:
		/** Инициализация рендеринга. Для каждой новой картинки необходимо заново инициализировать рендерер.
			Для ray-tracing: isDiffuse = 0, isBreakOnMaterial = true.
			Для path-tracing: isDiffuse = 1, isBreakOnMaterial = false. */
		StandardRendererWithPointLight(const Camera& camera, 
									   const Object& scene,
									   Image& img, 
									   int maxDepth, 
									   bool isDiffuse, 
									   bool isBreakOnMaterial);

		/** Рендерит в заданную картинку с заданной камерой, сценой и параметрами. Порядок рендеринга - случайные пиксели, это дает возможность хорошо предсказывать время рендеринга. */
		void render(void);

		/** Массив точечных источников освещения. Пользователь сам их задает, далее это учитывается при рендеринге. */
		std::vector<PointLight>	luminaries;

		/** Массив указателей на объекты порталов. Только таким образом можно учесть телепортацию точечных источников освещения через порталы, из сцены нельзя получить информацию о порталах. Так же получается, что пользователь сам задает когда ему надо учитывать в освещении портал, а когда не надо. */
		std::vector<Portals*>	portals;
	protected:
		int maxDepth;
		bool isDiffuse;
		bool isBreakOnMaterial;

		double time;

		/** Эти функции выводят информацию в консоль. */
		void onStartRender(void);
		void onEveryLine(double percent);
		void onEndRendering(void);

		Color computePixel(int x, int y);
		Color computeColor(Ray ray);
	};

	//-------------------------------------------------------------------------
	/** Рендерит так называемый ray-tracing. Особенности:
			Достаточно одного луча на пиксель.
			Малографонистое изображение.
			Поддержка точечных источников освещения(единственный источник освещения для данного рендеринга).
				Соответственно тени могут быть только точными.
				Учитываются полупрозрачные объекты.
				Учитываются порталы.
			Не поддерживает depth of field.
			Не поддерживает рассеянные тени и рассеянное освещение(например от неба, от источников освещения, имеющих форму).
	 */
	class RayTracing : public StandardRendererWithPointLight
	{
	public:
		RayTracing(const Camera& camera, 
				   const Object& scene,
				   Image& img, 
				   int aliasing = 1,
				   int maxDepth = 30);

	protected:
		int antialiasing;
		Color computePixel(int x, int y);
	};

	//-------------------------------------------------------------------------
	/** Рендерит так называемый path-tracing. Особенности:
			Недостаточно одного луча на пиксель.
				Чем больше лучей, тем лучше.
				Нормальное изображение - 400 лучей.
				Идеальное изображение - 10 000 лучей.
			Графонистое изображение.
			Поддержка точечных источников освещения(единственный источник освещения для данного рендеринга).
				Соответственно тени могут быть только точными.
				Учитываются полупрозрачные объекты.
				Учитываются порталы.
			Поддержка depth of field.
			Поддержка рассеянных теней и рассеянного освещения(например от неба, от источников освещения, имеющих форму).
	 */
	class PathTracing : public StandardRendererWithPointLight
	{
	public:
		PathTracing(const Camera& camera, 
					const Object& scene,
					Image& img, 
					int samples = 400,
					int maxDepth = 30);

	protected:
		int samples;
		Color computePixel(int x, int y);
	};

};

#endif