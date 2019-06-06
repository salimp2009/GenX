//#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "Hitable.hpp"
#include "Material.hpp"

// Sphere Equation; sphere center is at (cx,cy,cz)
// Equation to check if(x,y,z) is on the sphere;
// (x-cx)*(x-cx)+(y-cy)*(y-cy)+(z-cz)*(z-cz)=R*R 
// (x,y,z) is any point 
// (cx, cy,cz) is C, center of sphere 
// R is the radius of sphere
// Solving dot((p-c), (p-c))=R*R; equation to check if the ray hits the sphere in vector form
// p(t)= A+t*B; ray equation
// Solving the equation for unknown t; t*t*dot(B,B)+2*t*dot(B,A-C)+dot(A-C,A-C)-R*R =0

class Sphere:public Hitable
{
public:
	
	Sphere() :center{}, radius{ 0.0f }, mat_ptr{nullptr} { }
	Sphere(Vec3 cen, float r, Material* m) : center{ cen }, radius{ r }, mat_ptr{ m } { }
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;
	Vec3 center;
	float radius;
	Material* mat_ptr;
};

bool Sphere::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb{ center - Vec3{ radius, radius, radius }, center + Vec3{ radius, radius, radius } };
	return true;
}

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
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp= (-b + sqrt(b * b - a * c)) / a;			// second real root; tmax
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);		// point at t
			rec.normal = (rec.p - center) / radius;		// surface normal at hit point t
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}


// DELETE THIS SECTION; NEW HEADER FILE CREATED FOR MOVING_SPHERE!!!!!!
//// Added a moving sphere class for motion blur
//class Moving_Sphere :public Hitable
//{
//public:
//
//	Moving_Sphere() :center0{}, center1{}, time0{ 0.0f }, time1{ 0.0f }, radius{ 0.0f }, mat_ptr{ nullptr } { }
//	Moving_Sphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material* m) 
//		: center0{ cen0 }, center1{ cen1 }, time0{ t0 }, time1{t1}, radius{ r }, mat_ptr{ m } { }
//	~Moving_Sphere() { delete mat_ptr; }
//	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
//	Vec3 center(float time) const;		// center of the sphere changes  with time 
//	Vec3 center0, center1;				// center0 is the center at time 0 and center1 is center at time1
//	float time0, time1;					// time0 and time1 might be different then the shutter time 
//	float radius;
//	Material* mat_ptr;
//};
//
//Vec3 Moving_Sphere::center(float time) const
//{
//	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
//}

//// the center of the moving sphere is calculated at the time ray hits it
//bool Moving_Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
//{														// Constant "2" multipliers are eliminated due to redundancy
//	Vec3 oc = r.origin() - center(r.time());			// A-C; A is the origin of the ray, C is the center of sphere
//	float a = dot(r.direction(), r.direction());		// dot(B, B); B=r.direction()
//	float b = dot(oc, r.direction());					// originally; 2* dot(B, A-C) in original formula but here it is dot(A-C, B) which are equal
//	float c = dot(oc, oc) - radius * radius;			// dot(A-C, A-C)-R*R; R is the radius
//	float discriminant = b * b - a * c;					// if the above hit_sphere equation will be solved for t; // discriminant has to be positive to have real roots
//	if (discriminant > 0)								// if discriminant >0 there are two real roots; 
//	{
//		float temp = (-b - sqrt(b * b - a * c)) / a;	// first real root; tmin; closest hit point
//		if (temp<t_max && temp>t_min)
//		{
//			rec.t = temp;
//			rec.p = r.point_at_parameter(rec.t);
//			rec.normal = (rec.p - center(r.time())) / radius;		// the center of the moving sphere is calculated at the time ray hits it
//			rec.mat_ptr = mat_ptr;
//			return true;
//		}
//		temp = (-b + sqrt(b * b - a * c)) / a;						// second real root; tmax
//		if (temp<t_max && temp>t_min)
//		{
//			rec.t = temp;
//			rec.p = r.point_at_parameter(rec.t);					// point at t
//			rec.normal = (rec.p - center(r.time())) / radius;		// surface normal at hit point t
//			rec.mat_ptr = mat_ptr;
//			return true;
//		}
//	}
//	return false;
//}

#endif // SPHERE_H

 