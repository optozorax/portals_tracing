#include <pt/object/texture_polygon.h>

namespace pt
{

//-----------------------------------------------------------------------------
TexturePolygon::TexturePolygon(const std::vector<vec2>& polygon, crd3 coords, Image_ptr img, const space2& tr, double k_coef, double s_coef) : img(img), array(polygon), coords(coords), tr(tr), k_coef(k_coef), s_coef(s_coef) {
	d = -dot(coords.k, coords.pos);
	normal = coords.k;

	min.x = +std::numeric_limits<double>::infinity();
	min.y = +std::numeric_limits<double>::infinity();
	max.x = -std::numeric_limits<double>::infinity();
	max.y = -std::numeric_limits<double>::infinity();

	for (size_t i = 0; i < polygon.size(); i++) {
		if (polygon[i].x < min.x) min.x = polygon[i].x;
		if (polygon[i].y < min.y) min.y = polygon[i].y;
		if (polygon[i].x > max.x) max.x = polygon[i].x;
		if (polygon[i].y > max.y) max.y = polygon[i].y;
	}
}

//-----------------------------------------------------------------------------
bool TexturePolygon::intersect(const Ray& ray, 
					Intersection& inter, 
					double tMin, 
					double tMax) const {
	if (dot(ray.dir, normal) != 0) {
		double t = (-d - dot(ray.pos, normal))/dot(ray.dir, normal);

		if (t > tMin && t < tMax) {
			// Position when ray intersect plane
			vec3 x = ray.pos + ray.dir * t;

			vec2 r;
			r.x = (dot(x, coords.i) - dot(coords.pos, coords.i))/dot(coords.i, coords.i);
			r.y = (dot(x, coords.j) - dot(coords.pos, coords.j))/dot(coords.j, coords.j);

			bool inRect = r.x >= min.x && r.y >= min.y && r.x <= max.x && r.y <= max.y;
			if (!inRect)
				return false;

			vec2 imgPos = tr.to(r);
			
			// Преобразование координат из [0, 1]x[0, 1] в координаты изображения. Так же текстура делается повторяющейся.
			imgPos.x += int(std::fabs(imgPos.x) + 2);
			imgPos.y += int(std::fabs(imgPos.y) + 2);
			imgPos.x = std::fmodf(imgPos.x, 1);
			imgPos.y = std::fmodf(imgPos.y, 1);
			imgPos.x *= img->getWidth();
			imgPos.y *= img->getHeight();
			

			if (imgPos.x < 0 || imgPos.x > img->getWidth() ||
				imgPos.y < 0 || imgPos.y > img->getHeight())
				return false;
			if (img->operator()(imgPos.x, imgPos.y).a == 0)
				return false;

			if (pointInPolygon(array, r)) {
				inter.data.vector = imgPos;
				inter.t = t;
				if (dot(ray.dir, normal) < 0)
					inter.normal = normal;
				else
					inter.normal = -normal;
				inter.pos = x;
				return true;
			} else
				return false;
		} else
			// Ray is not in ranges
			return false;
	} else {
		// Ray is parallel plane
		return false;
	}
}

//-----------------------------------------------------------------------------
ScatterType TexturePolygon::scatter(const Ray& ray,
						 const Intersection& inter,
						 Color& clrAbsorbtion,
						 Ray& scattered,
						 double& diffusion) const {
	Scatter s(img->operator()(inter.data.vector.x, inter.data.vector.y), k_coef, s_coef);
	return s.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
}

};