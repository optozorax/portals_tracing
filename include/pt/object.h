#ifndef PT_OBJECT_H
#define PT_OBJECT_H

#include <pt/vector.h>
#include <pt/basics.h>

namespace pt
{

	struct Ray;
	struct Intersection;
	class Object;
	class Material;
	class Object;

	//-------------------------------------------------------------------------
	struct Ray
	{
		Vector pos;
		Vector dir;
	};

	//-------------------------------------------------------------------------
	struct Intersection
	{
		float t;
		Vector pos;
		Vector normal;
	};

	enum ScatterType
	{
		SCATTER_END, // Возвращается, если луч поглотился, или достигнут источник света. Если луч поглотился, то объект обязан записать в clrAbsorbtion нулевой цвет. Если достигнут источник света, то туда записывается цвет света.
		SCATTER_RAYTRACING_END, // Возвращается, если рейтрейсинг может остановиться на этом. Такое может возвращаться например для: обычного материала, подповерхностного рассеивания.
		SCATTER_NEXT // Возвращается, если луч может идти дальше, например для: искривителей лучей(черные дыры, червоточины), прозрачные объекты, отражающие объекты.
	};

	//-------------------------------------------------------------------------
	class Object
	{
	public:
		virtual ~Object() {}
		virtual bool intersect(const Ray& ray, 
							   Intersection& inter, 
							   float tMin, 
							   float tMax) = 0 const;

		/** Получает текущий луч, его пересечение с фигурой. Возвращает поглощенный цвет и направление отраженного луча. Функция возвращает true, если луч может идти дальше, false, если луч поглотился. Тогда, если он поглотился, в цвет поглощения обязано быть записано 0, если этот объект не является источником света. Если является источником света, то в цвет поголщения должен быть записан цвет свечения. */
		virtual ScatterType scatter(const Ray& ray,
									const Intersection& inter,
									Color& clrAbsorbtion,
									Ray& scattered,
									/** Показывает насколько сильно данный луч может рассеиваться в этом положении. Используется исключительно рендерерами. Объект должен каждый раз задавать этот параметр. */
									double& diffusion,
									bool& rayTracingStop) = 0 const;
	};

	//-------------------------------------------------------------------------
	class Material
	{
	public:
		virtual ~Material() {}
		virtual ScatterType scatter(const Ray& ray,
									const Intersection& inter,
									Color& clrAbsorbtion,
									Ray& scattered,
									double& diffusion) = 0 const;	
	};

	//-------------------------------------------------------------------------
	class Shape : public Object
	{
	public:
		Shape(Material* material) : material(material) {}

		virtual bool intersect(const Ray& ray, 
							   Intersection& inter, 
							   float tMin, 
							   float tMax) = 0 const;
		ScatterType scatter(const Ray& ray,
							const Intersection& inter,
							Color& clrAbsorbtion,
							Ray& scattered,
							double& diffusion) const final {
			return material->scatter(ray, inter, clrABsorbtion, scattered, diffusion);
		}	

		Material* material;
	};

};

#endif