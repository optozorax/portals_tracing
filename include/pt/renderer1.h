#ifndef PT_RENDERER1_H
#define PT_RENDERER1_H

#include <vector>

#include <pt/basics.h>
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
		PointLight(vec3 pos, Color clr) : pos(pos), clr(clr.sqrt()) {}

		vec3 pos;
		Color clr;
	};

	//-------------------------------------------------------------------------
	/** Абстрактный класс рендерера. */
	class Renderer1
	{
	public:
		Renderer1() {}
		virtual ~Renderer1() {}

		void assign(Camera* camera1, Object* scene1, Image* img1) {
			camera = camera1;
			scene = scene1;
			img = img1;
		}

		virtual void render(void) = 0;

		virtual Color computePixel(int x, int y) const = 0;
		virtual Color computeColor(Ray ray) const = 0;
	protected:
		Camera*	camera;
		Object*	scene;
		Image*	img;

		virtual void onStartRender(void) {}
		virtual void onEveryLine(double percent) const {}
		virtual void onEndRendering(void) const {}
	};

	//-------------------------------------------------------------------------
	/** Стандартный класс рендеринга, на основе которого строятся ray- и path-tracing. Имеет в себе не только простой рендеринг, но еще и фичу: точечные источники освещения, которые поддерживают не только полупрозрачные объекты, но еще и порталы. Вообще освещение можно было бы моделировать при помощи сфер с материалом light, но это будет слишком долго. */
	class StandardRenderer : public Renderer1
	{
	public:
		/** Инициализация рендеринга. Для каждой новой картинки необходимо заново инициализировать рендерер.
			Для ray-tracing: isDiffuse = false, isBreakOnMaterial = true.
			Для path-tracing: isDiffuse = true, isBreakOnMaterial = false. */
		StandardRenderer(int threads,
						 int maxDepth,
						 double tMax, 
						 bool isDiffuse, 
						 bool isBreakOnMaterial,
						 bool isWriteText);

		~StandardRenderer();

		/** Рендерит в заданную картинку с заданной камерой, сценой и параметрами. Порядок рендеринга - случайные пиксели, это дает возможность хорошо предсказывать время рендеринга. */
		void render(void);

		/** Добавляет во внутреннее хранилище портал для последующей обработки. */
		void addPortal(Portals* portal);

		/** Удаляет все порталы из обработки рендером. */
		void clearPortals(void);

		Color computePixel(int x, int y) const;
		Color computeColor(Ray ray) const;

		/** Массив точечных источников освещения. Пользователь сам их задает, далее это учитывается при рендеринге. */
		std::vector<PointLight>	luminaries;
	protected:
		std::vector<Portals*> portals;
		std::vector<Portals*> invertedPortals;

		int threads;
		int maxDepth;
		double tMax;
		bool isDiffuse;
		bool isBreakOnMaterial;
		bool isWriteText;

		double time;

		/** Эти функции выводят информацию в консоль. */
		void onStartRender(void);
		void onEveryLine(double percent) const;
		void onEndRendering(void) const;

		/** Считает все возможное освещение от точечных источников освещения в данной позиции сцены, при этом учитывается наличие порталов, а так же то, что объекты могут быть полупрозрачны. */
		Color computeLight(vec3 pos, vec3 normal,
						   std::vector<std::pair<Portals*, vec3> >& teleportation,
						   int depth) const;
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
	class RayTracing : public StandardRenderer
	{
	public:
		RayTracing(int aliasing = 1,
				   int threads = 1,
				   bool isWriteText = true,
				   int maxDepth = 30,
				   double tMax = 100000);

	protected:
		int antialiasing;
		Color computePixel(int x, int y) const;
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
	class PathTracing : public StandardRenderer
	{
	public:
		PathTracing(int samples = 400,
					int threads = 1,
					bool isWriteText = true,
					int maxDepth = 30,
					double tMax = 100000);

	protected:
		int samples;
		Color computePixel(int x, int y) const;
	};

};

#endif