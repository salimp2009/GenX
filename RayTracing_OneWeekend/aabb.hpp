//#pragma once
#ifndef AABB_H
#define AABB_H
#include "Ray.hpp"
#include "Hitable.hpp"

// p(t) =A+ tB; ray equation
// That equation applies to all three of the ​x / y / z
// coordinates. For example ​x(t) = Ax + t * Bx.
// This ray hits the plane ​x = x0
// at the t that satisfies this equation:
// x0 = Ax + t0 * Bx

// Thus t at that hitpoint is :
// t0 = (x0 - Ax) / Bx
// ​t1 = (x1 - Ax) / Bx for ​x1
// tx0 = min(​(x0 - Ax) / Bx, (x1 - Ax) / Bx); tx1 = max(​(x0 - Ax) / Bx, (x1 - Ax) / Bx); 

// Overlap function psuecode;
// bool overlap(d, D, e, E, f, F)
// f = max(d, e) 
// F = min(D, E)
// return (f < F) 

inline float ffmin(float a, float b) { return a < b ? a : b; };
inline float ffmax(float a, float b) { return a > b ? a : b; };

class aabb
{
public:
	aabb() {}
	aabb(const Vec3& a, const Vec3& b) : _min{a}, _max{b} {}

	Vec3 min() const { return _min; }
	Vec3 max() const { return _max; }

	bool hit(const Ray& r, float tmin, float tmax) const
	{
		for (int a{0}; a < 3; ++a)
		{
			float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);			 
							 																										
			float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
							 
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

	bool hit_alt(const Ray& r, float tmin, float tmax) const;

	Vec3 _min;
	Vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	Vec3 small{ ffmin(box0.min().x(), box1.min().x()),
				ffmin(box0.min().y(), box1.min().y()),
				ffmin(box0.min().z(), box1.min().z()) };
	Vec3 big{ ffmax(box0.max().x(), box1.max().x()),
				ffmax(box0.max().y(), box1.max().y()),
				ffmax(box0.max().z(), box1.max().z()) };
	return aabb{ small, big };
}

// Andrew Kensler's proposed version of the hit; which works better on most compilers; test it!!!
inline bool aabb::hit_alt(const Ray& r, float tmin, float tmax) const
{
	for (int a{0}; a < 3; ++a)
	{
		float invD = 1.0f / r.direction()[a];
		float t0 = (_min[a] - r.origin()[a]) * invD;   
		float t1 = (_max[a] - r.origin()[a]) * invD;
		if (invD < 0.0f)								// if the ray is travelling in the negative t0 and t1 will reversed			
			std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;			
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)								// the ray does not hit the box
			return false;
	}
	return true;
}


#endif // !AABB_H
