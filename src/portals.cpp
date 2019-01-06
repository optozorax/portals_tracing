#include <prtl/portal.h>
#include <pt/shape/portals.h>

namespace pt
{

//-----------------------------------------------------------------------------
Portals::Portals(crd3 c1, crd3 c2, std::vector<vec2> poly, Material* first, Material* second) : p1(c1), p2(c2), poly(poly), first(first), second(second), pg1(poly, c1, nullptr), pg2(poly, c2, nullptr) {
}

//-----------------------------------------------------------------------------
Portals::~Portals() {
}

//-----------------------------------------------------------------------------
void Portals::assign(crd3 c1, crd3 c2, std::vector<vec2> poly1, Material* first1, Material* second1) {
	pg1.assign(poly1, c1, nullptr);
	pg2.assign(poly1, c2, nullptr);
	p1 = c1;
	p2 = c2;
	poly = poly1;
	first = first1;
	second = second1;
}

//-----------------------------------------------------------------------------
bool Portals::intersect(const Ray& ray, 
						Intersection& inter, 
						double tMin, 
						double tMax) const {
	Intersection inter1, inter2;
	bool isFirst = pg1.intersect(ray, inter1, tMin, tMax);
	bool isSecond = pg2.intersect(ray, inter2, tMin, tMax);

	isFirst &= inter1.t >= tMin && inter1.t <= tMax;
	isSecond &= inter2.t >= tMin && inter2.t <= tMax;
	bool firstLessSecond = inter1.t < inter2.t;

	if (isFirst && isSecond && firstLessSecond) {
		inter = inter1;
		inter.data.integer = 1;
		return true;
	}

	if (isFirst && isSecond && !firstLessSecond) {
		inter = inter2;
		inter.data.integer = 2;
		return true;
	}	

	if (isFirst) {
		inter = inter1;
		inter.data.integer = 1;
		return true;
	}

	if (isSecond) {
		inter = inter2;
		inter.data.integer = 2;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
ScatterType Portals::scatter(const Ray& ray,
							 const Intersection& inter,
							 Color& clrAbsorbtion,
							 Ray& scattered,
							 double& diffusion) const {
	if (inter.data.integer == 1)
		if (dot(inter.normal, p1.k) > 0)
			return first->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);
	if (inter.data.integer == 2)
		if (dot(inter.normal, p2.k) < 0)
			return second->scatter(ray, inter, clrAbsorbtion, scattered, diffusion);

	scattered.pos = inter.pos;
	scattered.dir = ray.dir;
	if (inter.data.integer == 1) {
		prtl::portal3 portal(p1, p2);
		scattered.pos = portal.teleport(scattered.pos);
		scattered.dir = portal.teleportDir(scattered.dir);
	} else {
		prtl::portal3 portal(p2, p1);
		scattered.pos = portal.teleport(scattered.pos);
		scattered.dir = portal.teleportDir(scattered.dir);
	}
	clrAbsorbtion = Color(1, 1, 1, 1);
	diffusion = 0;
	return SCATTER_NEXT;
}

//-----------------------------------------------------------------------------
Portals invert(Portals a) {
	crd3 coords1 = a.p2;
	coords1.k = -coords1.k;
	crd3 coords2 = a.p1;
	coords2.k = -coords2.k;
	return Portals(coords1, coords2, a.poly, a.second, a.first);
}

}