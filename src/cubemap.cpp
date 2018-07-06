#include <pt/object/cubemap.h>

namespace pt
{

//-----------------------------------------------------------------------------
void convert_xyz_to_cube_uv(double x, double y, double z, int *index, double *u, double *v) {
	// Thanks for Wikipedia: https://en.wikipedia.org/wiki/Cube_mapping
	double absX = fabs(x);
	double absY = fabs(y);
	double absZ = fabs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	double maxAxis, uc, vc;

	// POSITIVE X
	if (isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -z;
		vc = y;
		*index = 0;
	}
	// NEGATIVE X
	if (!isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = z;
		vc = y;
		*index = 1;
	}
	// POSITIVE Y
	if (isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = x;
		vc = -z;
		*index = 2;
	}
	// NEGATIVE Y
	if (!isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = x;
		vc = z;
		*index = 3;
	}
	// POSITIVE Z
	if (isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = x;
		vc = y;
		*index = 4;
	}
	// NEGATIVE Z
	if (!isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -x;
		vc = y;
		*index = 5;
	}

	// Convert range from -1 to 1 to 0 to 1
	*u = 0.5f * (uc / maxAxis + 1.0f);
	*v = 0.5f * (vc / maxAxis + 1.0f);
}

//-----------------------------------------------------------------------------
CubeMap::CubeMap(std::string bmp) {
	std::wstring wstr(bmp.begin(), bmp.end());
	m_image.ReadFromFile(wstr.c_str());
	m_size = m_image.TellWidth()/4;
}

//-----------------------------------------------------------------------------
bool CubeMap::intersect(const Ray& ray, 
					Intersection& inter, 
					double tMin, 
					double tMax) const {
	inter.t = tMax;
	inter.pos = ray.pos + ray.dir * inter.t;
	inter.normal = ray.dir;
	return true;
}

//-----------------------------------------------------------------------------
ScatterType CubeMap::scatter(const Ray& ray,
						 const Intersection& inter,
						 Color& clrAbsorbtion,
						 Ray& scattered,
						 double& diffusion) const {
	vec3 dir = ray.dir;
	int index = 0;
	double u = 0, v = 0;

	convert_xyz_to_cube_uv(-dir.x, dir.z, dir.y, &index, &u, &v);
	v = 1 - v;
	u *= m_size;
	v *= m_size;

	switch (index) {
		case 0: {
			u += m_size * 2;
			v += m_size;
		} break;
		case 1: {
			v += m_size;
		} break;
		case 2: {
			u += m_size;
		} break;
		case 3: {
			u += m_size;
			v += m_size * 2;
		} break;
		case 4: {
			u += m_size;
			v += m_size;
		} break;
		case 5: {
			u += m_size * 3;
			v += m_size;
		} break;
	}

	auto pix = m_image.GetPixel(u, v);

	clrAbsorbtion.a = 1;
	clrAbsorbtion.r = pix.Red/255.0;
	clrAbsorbtion.g = pix.Green/255.0;
	clrAbsorbtion.b = pix.Blue/255.0;
	clrAbsorbtion.r *= clrAbsorbtion.r;
	clrAbsorbtion.g *= clrAbsorbtion.g;
	clrAbsorbtion.b *= clrAbsorbtion.b;
	diffusion = 0;
	
	return SCATTER_END;
}

};