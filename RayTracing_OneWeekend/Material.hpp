//#pragma once
#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "Hitable.hpp"
#include <random>
#include <cmath>
#include "texture.hpp"

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

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)
{
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt*(uv - n * dt) - n * std::sqrt(discriminant);
		return true;
	} 
	else
	{
		return false;
	}
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1.0f - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1.0f-r0)*pow((1-cosine),5);
}

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
	virtual ~Material() = default;
};

class Lambertian: public Material
{
public:
	Lambertian() = default;
	Lambertian(Texture* a):albedo{a} { }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray{ rec.p, target - rec.p, r_in.time() };
		attenuation = albedo->value(0.0f, 0.0f, rec.p);
		return true;
	}
	Texture* albedo;
};

class Metal: public Material
{
public:
	Metal(const Vec3& a, float f): albedo{a} 
	{ 
		if (f < 1.0f)
			fuzz = f;
		else
			fuzz = 1.0f;
	}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray{ rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time() }; // added r_in.timr(); forgotten in the original code
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3 albedo;
	float fuzz;
};

class dielectric : public Material
{
public:
	dielectric(float ri): ref_idx{ri} { }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3{ 1.0f, 1.0f, 1.0f };
		Vec3 refracted;
		float reflect_prob;
		float cosine;
	
		std::random_device rd;									// will be used to obtain a seed for random number engine
		std::mt19937 gen(rd());									// mersenne_twister_engine seeded with rd()	; 				
		std::uniform_real_distribution<float>dis(0.0f, 1.0f);

		if (dot(r_in.direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			//cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine= dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = sqrt(1.0f - ref_idx * ref_idx) * (1.0f - cosine * cosine);
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0f;
		}
		if (dis(gen)<reflect_prob)
		{
			scattered = Ray{ rec.p, reflected, r_in.time() }; // added r_in.timr(); forgotten in the original code	
		}
		else
		{
			scattered = Ray{ rec.p, refracted, r_in.time() };   // added r_in.timr(); forgotten in the original code
		}
		return true;
	}

	float ref_idx;
};


#endif // !MATERIAL_H_

