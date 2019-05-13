//#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_
#include "Ray.hpp"

class Camera
{
public:
	Camera() 
		: origin{ 0.0f, 0.0f, 0.0f }, lower_left_corner{ -2.0f, -1.0f, -1.0f }, 
		  horizontal{ 4.0f, 0.0f, 0.0f }, vertical { 0.0f, 2.0f, 0.0f } { }
	Ray get_ray(float u, float v) 
	{
		return Ray { origin, lower_left_corner + u * horizontal + v * vertical-origin };
	}
	
	~Camera(){}

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};


#endif // !CAMERA_H_

