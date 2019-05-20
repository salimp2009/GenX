//#pragma once
#ifndef  _VEC3_H_
#define _VEC3_H_
#include <iostream>
#include <cstdlib>
#include <cmath>

//  "RayTracing Over a Weekend" by Peter Shirley

class Vec3
{
public:
	Vec3() : e{ 0.0f, 0.0f, 0.0f } { }
//	Vec3() {e[0]={0.0f}; e[1]={0.0f}; e[2]={0.0f};}                        // original way in the code to initialize

	Vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } { }
//	Vec3(float e0, float e1, float e2){ e[0]={e0}; e[1]={e1}; e[2]={e2};}  // original way in the code to initialize
	~Vec3() = default;

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3{ -e[0], -e[1], -e[2] }; }
	inline float operator[](int i) const {return e[i];}
	inline float& operator[](int i) { return e[i]; }

	inline Vec3& operator+=(const Vec3& v2);
	inline Vec3& operator-=(const Vec3& v2);
	inline Vec3& operator*=(const Vec3& v2);
	inline Vec3& operator/=(const Vec3& v2);
	inline Vec3& operator*=(const float t);
	inline Vec3& operator/=(const float t);

	inline float length() const {
		return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);}
	inline float squared_length() const {
		return  e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector();

	// public data member; x,y,z coordinates
	float e[3];                                // better to use float x, y, z instead of e[3] array																	// or std::array<float,3>e;
};

inline std::istream& operator>>(std::istream& is, Vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void Vec3::make_unit_vector() {
	float k = 1.0f/(std::sqrt( e[0]*e[0]+e[1]*e[1]+e[2]*e[2]));
	e[0] *= k; 
	e[1] *= k;
	e[2] *= k;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3{ v1.e[0] + v2.e[0] , v1.e[1] + v2.e[1], v1.e[2] + v2.e[2] };
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3{ v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2] };
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return Vec3{ v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2] };
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
	return Vec3{ v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2] };
}

inline Vec3 operator*(float t, const Vec3& v) {
	return Vec3{ t * v.e[0], t * v.e[1], t * v.e[2] };
}

inline Vec3 operator/(const Vec3& v, float t) {
	return Vec3{ v.e[0] / t, v.e[1] / t, v.e[2] / t };
}

inline Vec3 operator*(const Vec3& v, float t) {
	return Vec3{ t * v.e[0], t * v.e[1], t * v.e[2] };
}

inline float dot(const Vec3& v1, const Vec3 & v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

// right-handed 
inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3{ (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]), 
				 (-(v1.e[0] * v2.e[2]- v1.e[2]*v2.e[0])),    // (v1.e[2]*v2.e[0] - v1.e[0] * v2.e[2]); this is the way i prefer				 
				(v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0])};
}

inline Vec3& Vec3::operator+=(const Vec3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
	float k = 1.0f / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vec3 unit_vector(Vec3 v) {
	return v / v.length();
}



#endif // ! _VEC3_H_
