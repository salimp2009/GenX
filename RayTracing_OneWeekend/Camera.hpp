//#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_
#include "Ray.hpp"
#include <cmath>
#include <corecrt_math_defines.h>

class Camera
{
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup,  float vfov, float aspect)		// vfov is top to bottom in degrees;
	{										// vertical field of view h = tan(theta / 2)
		Vec3 u, v, w;
		float theta =  vfov * M_PI/ 180.0f;	// M_PI is a Windows specific macro PI number;  #define _USE_MATH_DEFINES 
		float half_height = tan(theta / 2.0f);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		//lower_left_corner = Vec3{ -half_width, -half_height, -1.0f };
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2.0f * half_width * u;
		vertical = 2.0f * half_height * v;
	}
	
	Ray get_ray(float s, float t) 
	{
		return Ray { origin, lower_left_corner + s * horizontal + t * vertical-origin };
	}
	
	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif // !CAMERA_H_

