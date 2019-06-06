//#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "vec3.hpp"

class Texture
{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
	virtual ~Texture() = default;
};

class Constant_Texture: public Texture
{
public:
	Constant_Texture() :color{} {}
	Constant_Texture(Vec3 c): color{c} { }
	virtual Vec3 value(float u, float v, const Vec3& p) const override
	{
		return color;
	}
	Vec3 color;
};

// 3D checker pattern; sign of sine and cosine alternates regularly; 
// mulptiplying those trig functions in all 3dimensions forms a 3D checker pattern
// we can either pass a constant_texture or any other procedural texture to the even and odd textures 
// to make a combination (Pat Hanrahan, 1980)
class Checker_Texture:public Texture
{
public:
	Checker_Texture() :even{ }, odd{ } { }
	Checker_Texture(Texture* t0, Texture* t1) : even{t0}, odd{t1} { }
	virtual Vec3 value(float u, float v, const Vec3& p) const override
	{
		float sines = std::sin(10.0f*p.x()) * std::sin(10.0f*p.y()) * std::sin(10.0f*p.z());  
		if (sines < 0.0f)
		{
			return odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}
	Texture* even;
	Texture* odd;
};


#endif // TEXTURE_H

