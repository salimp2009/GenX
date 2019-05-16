//#pragma once
#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "Hitable.hpp"
#include <random>

Vec3 random_in_unit_sphere()
{
	std::random_device rd;									// will be used to obtain a seed for random number engine
	std::mt19937 gen(rd());									// mersenne_twister_engine seeded with rd()	; 				
	std::uniform_real_distribution<float>dis(0.0f, 1.0f);
	Vec3 p;
	do {
		p = 2.0f * Vec3(dis(gen), dis(gen), dis(gen)) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2.0f * dot(v, n) * n;
}

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian: public Material
{
public:
	Lambertian(const Vec3& a):albedo{a} { }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	
	Vec3 albedo;
};

class Metal: public Material
{
public:
	Metal(const Vec3& a): albedo{a} { }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3 albedo;
};


#endif // !MATERIAL_H_

