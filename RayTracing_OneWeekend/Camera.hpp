//#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_
#include "Ray.hpp"
#include <cmath>
#include <corecrt_math_defines.h>
#include <random>

Vec3 random_in_unit_disk()
{
	std::random_device rd;									// will be used to obtain a seed for random number engine
	std::mt19937 gen(rd());									// mersenne_twister_engine seeded with rd()	; 				
	std::uniform_real_distribution<float>dis(0.0f, 1.0f);
	Vec3 p;
	do {
		p = 2.0f * Vec3(dis(gen), dis(gen), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
	} while (dot(p,p) >= 1.0f);
	return p;
}

class Camera
{
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup,  float vfov, float aspect, float aperture, float focus_dist, float t0, float t1)						
	{												// vertical field of view h = tan(theta / 2), // vfov is top to bottom in degrees;
		time0 = t0;									// t0 and t1 added for motion blur to record the time shutter is open and closed
		time1 = t1;
		lens_radius = aperture / 2.0f;
		float theta =  vfov * M_PI/ 180.0f;			// M_PI is a Windows specific macro PI number;  #define _USE_MATH_DEFINES 
		float half_height = tan(theta / 2.0f);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width *focus_dist* u - half_height*focus_dist*v - focus_dist*w;
		horizontal = 2.0f*half_width*focus_dist*u;
		vertical = 2.0f*half_height*focus_dist*v;
	}
	
	Ray get_ray(float s, float t) 
	{
		std::random_device rnd;									// will be used to obtain a seed for random number engine
		std::mt19937 gen(rnd());								// mersenne_twister_engine seeded with rd()	; 				
		std::uniform_real_distribution<float>dis(0.0f, 1.0f);
		
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		float time = time0 + dis(gen) * (time1 - time0);		// camera will generate rays at a random time between time1 and time2
		return Ray {origin + offset, lower_left_corner + s * horizontal + t * vertical-origin-offset, time};
	}
	
	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float time0;				// shutter open time; for motion blur
	float time1;				// shutter closed time; for motion blur
	float lens_radius;
	
};

#endif // !CAMERA_H_

