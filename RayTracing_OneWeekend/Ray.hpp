//#pragma once
#ifndef  _RAY_H_
#define	 _RAY_H_
#include "vec3.hpp"

class Ray {
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b): A{a}, B{b} { }
	Vec3 origin() const { return A; }			// A is the ray origin; point vector
	Vec3 direction() const { return B; }		// B is the ray direction vector
	Vec3 point_at_parameter(float t) const { return A + t * B; }	// p(t)=A + t*B is a ray function; p is a 3D position along aline 
																	// t is real number; p(t) moves the point along the ray				
																	// for positive t you get part of in front of A; called ray or half-line
	Vec3 A;															
	Vec3 B;
};

#endif // ! _RAY_H_
