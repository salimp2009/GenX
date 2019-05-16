//#pragma once
#ifndef HITABLE_H
#define HITABLE_H
#include "Ray.hpp"

class Material;

struct  hit_record 
{
	float t{ 0.0f };
	Vec3 p;
	Vec3 normal;
	Material* mat_ptr;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif // !HITABLE_H
