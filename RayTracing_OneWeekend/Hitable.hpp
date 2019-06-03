//#pragma once
#ifndef HITABLE_H
#define HITABLE_H
#include "Ray.hpp"
#include "aabb.hpp"

class Material;

struct  hit_record 
{
	float t{ 0.0f };
	Vec3 p;
	Vec3 normal;
	Material* mat_ptr{ nullptr };
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
	virtual ~Hitable() = default;
};


#endif // !HITABLE_H
