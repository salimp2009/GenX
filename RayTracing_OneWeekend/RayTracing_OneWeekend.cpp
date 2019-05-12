#include <iostream>
#include <string>
#include <fstream>
#include "Ray.hpp"
#include "hitable_list.hpp"
#include "Sphere.hpp"
#include "float.h"

// Ray Tracer Project from "RayTracing Over a Weekend" by Peter Shirley
// Sample PPM color chart from "RayTracing Over a Weekend" by Peter Shirley
// Vec3 completed
// Ray.hpp added;

void vec3_test() 
{
	// test cases for Vec3
	Vec3 points{ 1.1f,1.2f,1.3f };
	Vec3 point2{ 3.0f, 4.0f, 5.0f };
	std::cout << "\nPoints: ";
	std::cout << points.x() << " " << points.y() << " " << points.z() << '\n';
	std::cout << points.length() << '\n';
	Vec3 point3 = -point2;
	std::cout << point3[0] << '\n';
	point3[2] = 10;
	std::cout << point3[2] << '\n';
	std::cout << point2 << '\n';

	point3.make_unit_vector();
	std::cout << point3 << '\n';
}

void ppm_test()
{
	std::string file{ "ppmexample.ppm" };
	std::ofstream ost(file);

	int nx{ 200 };
	int ny{ 100 };

	ost << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j)
	{
		for (int i = 0; i < nx; ++i)
		{
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;
			Vec3 col{ float(i) / float(nx), float(j) / float(ny), 0.2f };

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

		//	std::cout << ir << " " << ig << " " << ib << '\n';
			ost << ir << " " << ig << " " << ib << "\n";
		}
	}
	ost.close();
}

// Sphere Equation; sphere center is at (cx,cy,cz)
// Equation to check if(x,y,z) is on the sphere;
// (x-cx)*(x-cx)+(y-cy)*(y-cy)+(z-cz)*(z-cz)=R*R 
// (x,y,z) is any point 
// (cx, cy,cz) is C, center of sphere 
// R is the radius of sphere
// Solving dot((p-c), (p-c))=R*R; equation to check if the ray hits the sphere in vector form
// p(t)= A+t*B; ray equation
// Solving the equation for unknown t; t*t*dot(B,B)+2*t*dot(B,A-C)+dot(A-C,A-C)-R*R =0
float hit_sphere(const Vec3& center, float radius, const Ray& r) 
{
	Vec3 oc = r.origin() - center;							// A-C; A is the origin of the ray, C is the center of sphere
	float a = dot(r.direction(), r.direction());			// dot(B, B); B=r.direction()
	float b = 2 * dot(oc, r.direction());					// dot(B, A-C) in original formula but here it is dot(A-C, B) which are equal
	float c = dot(oc, oc)-radius*radius;					// dot(A-C, A-C)-R*R; R is the radius
	float discriminant = b * b - 4 * a * c;					// if the above hit_sphere equation will be solved for t; // discriminant has to be positive to have real roots
	if (discriminant < 0) {		
		return -1.0f;										// condition if no real roots; the color of the sphere will be blue-white gradient
	}
	else 
	{
		return (-b-sqrt(discriminant)) /(2.0f*a);			// tmin=-b-sqrt(b*b-4*a*c) / 2*a
	}
								
}

// Adding Surface Normal to color function; 
// Normal is calculated as unit vector in the direction of the hitpoint minus the center; P-C 

Vec3 color(const Ray& r, Hitable *world)
{
	hit_record rec;
	if(world->hit(r, 0.0f, FLT_MAX, rec)) 
	{
		return 0.5f* Vec3(rec.normal.x() + 1.0f, rec.normal.y()+1.0f, rec.normal.z()+1.0f);
	}
																						   
	Vec3 unit_direction = unit_vector(r.direction());											
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	//vec3_test();
	//ppm_test();

	// testing ray class; graphic output as PPM file
	std::string file{ "rayexample.ppm" };
	std::ofstream ost(file);

	int nx{ 200 };
	int ny{ 100 };
	ost << "P3\n" << nx << " " << ny << "\n255\n";
	Vec3 lower_left_corner{ -2.0f, -1.0f, -1.0f };
	Vec3 horizontal{ 4.0f, 0.0f, 0.0f };
	Vec3 vertical{ 0.0f, 2.0f, 0.0f };
	Vec3 origin{ 0.0f, 0.0f, 0.0f };
	Hitable* list[2];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
	Hitable * world = new Hitable_list(list, 2);

	for (int j = ny - 1; j >= 0; --j)
	{
		for (int i = 0; i < nx; ++i)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r{ origin, lower_left_corner + u * horizontal + v * vertical };

			Vec3 p = r.point_at_parameter(2.0f);
			Vec3 col = color(r, world);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			//   std::cout<<ir<<" "<<ig<<" "<<ib<<'\n';
			ost << ir << " " << ig << " " << ib << "\n";
		}
	}
	ost.close();


	delete list[0];
	delete list[1];
	
	world = nullptr;
	delete world;

	return 0;
}

