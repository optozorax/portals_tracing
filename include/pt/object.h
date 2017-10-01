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
		/** Показывает насколько сильно данный луч может рассеиваться в этом положении. Используется исключительно рендерерами. Объект должен каждый раз задавать этот параметр. */
		double diffusion;
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
		virtual bool scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered) = 0 const;
	};

	//-------------------------------------------------------------------------
	class Material
	{
	public:
		virtual ~Materail() {}
		virtual bool scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered) = 0 const;	
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
		bool scatter(const Ray& ray,
					 const Intersection& inter,
					 Color& clrAbsorbtion,
					 Ray& scattered) const final {
			return materail->scatter(ray, inter, clrABsorbtion, scattered);
		}	

		Material* material;
	};

};

#endif