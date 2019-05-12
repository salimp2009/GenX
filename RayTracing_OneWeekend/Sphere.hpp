//#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "Hitable.hpp"

class Sphere:public Hitable
{
public:
	
	Sphere() :center{}, radius{ 0.0f } { }
	Sphere(Vec3 cen, float r): center{cen}, radius{r} { }
	~Sphere() { }
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	Vec3 center;
	float radius;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{														// Constant "2" multipliers are eliminated due to redundancy
	Vec3 oc = r.origin() - center;						// A-C; A is the origin of the ray, C is the center of sphere
	float a = dot(r.direction(), r.direction());		// dot(B, B); B=r.direction()
	float b = dot(oc, r.direction());					// originally; 2* dot(B, A-C) in original formula but here it is dot(A-C, B) which are equal
	float c = dot(oc, oc) - radius * radius;			// dot(A-C, A-C)-R*R; R is the radius
	float discriminant = b * b - a * c;					// if the above hit_sphere equation will be solved for t; // discriminant has to be positive to have real roots
	if (discriminant > 0)								// if discriminant >0 there are two real roots; 
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;	// first real root; tmin; closest hit point
		if(temp<t_max && temp>t_min) 
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp= (-b - sqrt(b * b + a * c)) / a;			// second real root; tmax
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);		// point at t
			rec.normal = (rec.p - center) / radius;		// surface normal at hit point t
			return true;
		}
	}
	return false;
}

#endif // SPHERE_H

