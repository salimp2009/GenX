#include <iostream>
#include <string>
#include <fstream>
#include "Ray.hpp"
#include "hitable_list.hpp"
#include "Sphere.hpp"
#include "float.h"
#include "Camera.hpp"
#include "Material.hpp"
#include <random>

// Ray Tracer Project from "RayTracing Over a Weekend" by Peter Shirley
// Sample PPM color chart from "RayTracing Over a Weekend" by Peter Shirley
// Vec3 completed
// Ray.hpp added;

//void vec3_test() 
//{
//	// test cases for Vec3
//	Vec3 points{ 1.1f,1.2f,1.3f };
//	Vec3 point2{ 3.0f, 4.0f, 5.0f };
//	std::cout << "\nPoints: ";
//	std::cout << points.x() << " " << points.y() << " " << points.z() << '\n';
//	std::cout << points.length() << '\n';
//	Vec3 point3 = -point2;
//	std::cout << point3[0] << '\n';
//	point3[2] = 10;
//	std::cout << point3[2] << '\n';
//	std::cout << point2 << '\n';
//
//	point3.make_unit_vector();
//	std::cout << point3 << '\n';
//}

//void ppm_test()
//{
//	std::string file{ "ppmexample.ppm" };
//	std::ofstream ost(file);
//
//	int nx{ 200 };
//	int ny{ 100 };
//
//	ost << "P3\n" << nx << " " << ny << "\n255\n";
//	for (int j = ny - 1; j >= 0; --j)
//	{
//		for (int i = 0; i < nx; ++i)
//		{
//			float r = float(i) / float(nx);
//			float g = float(j) / float(ny);
//			float b = 0.2f;
//			Vec3 col{ float(i) / float(nx), float(j) / float(ny), 0.2f };
//
//			int ir = int(255.99 * col[0]);
//			int ig = int(255.99 * col[1]);
//			int ib = int(255.99 * col[2]);
//
//		//	std::cout << ir << " " << ig << " " << ib << '\n';
//			ost << ir << " " << ig << " " << ib << "\n";
//		}
//	}
//	ost.close();
//}

// Sphere Equation; sphere center is at (cx,cy,cz)
// Equation to check if(x,y,z) is on the sphere;
// (x-cx)*(x-cx)+(y-cy)*(y-cy)+(z-cz)*(z-cz)=R*R 
// (x,y,z) is any point 
// (cx, cy,cz) is C, center of sphere 
// R is the radius of sphere
// Solving dot((p-c), (p-c))=R*R; equation to check if the ray hits the sphere in vector form
// p(t)= A+t*B; ray equation
// Solving the equation for unknown t; t*t*dot(B,B)+2*t*dot(B,A-C)+dot(A-C,A-C)-R*R =0

//float hit_sphere(const Vec3& center, float radius, const Ray& r) 
//{
//	Vec3 oc = r.origin() - center;							// A-C; A is the origin of the ray, C is the center of sphere
//	float a = dot(r.direction(), r.direction());			// dot(B, B); B=r.direction()
//	float b = 2 * dot(oc, r.direction());					// dot(B, A-C) in original formula but here it is dot(A-C, B) which are equal
//	float c = dot(oc, oc)-radius*radius;					// dot(A-C, A-C)-R*R; R is the radius
//	float discriminant = b * b - 4 * a * c;					// if the above hit_sphere equation will be solved for t; // discriminant has to be positive to have real roots
//	if (discriminant < 0) {		
//		return -1.0f;										// condition if no real roots; the color of the sphere will be blue-white gradient
//	}
//	else 
//	{
//		return (-b-sqrt(discriminant)) /(2.0f*a);			// tmin=-b-sqrt(b*b-4*a*c) / 2*a
//	}
//								
//}


// Adding Surface Normal to color function; 
// Normal is calculated as unit vector in the direction of the hitpoint minus the center; P-C 
Vec3 color(const Ray& r, Hitable *world, int depth)
{
	hit_record rec;
	if(world->hit(r, 0.001f, FLT_MAX, rec)) 
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return Vec3{ 0.0f, 0.0f, 0.0f };
		}
		
	}
	else
	{
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}
																						   
	
}

int main()
{
	//vec3_test();
	//ppm_test();

	// testing ray class; graphic output as PPM file
	std::string file{ "rayexample.ppm" };
	std::ofstream ost(file);

	int nx{ 200 };			// canvas width; width of camera view area
	int ny{ 100 };			// canvas height; height of camera view area
	int ns{ 100 };			// number random rays for sampling colors in each pixel
	
	ost << "P3\n" << nx << " " << ny << "\n255\n";
	
	Hitable* list[4];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vec3{ 0.8f, 0.3f, 0.3f }));
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vec3{ 0.8f, 0.3f, 0.0f }));
	list[2] = new Sphere(Vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vec3{ 0.8f, 0.6f, 0.2f }));
	list[3] = new Sphere(Vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vec3{ 0.8f, 0.8f, 0.8f }));
	Hitable * world = new Hitable_list(list, 4);
	Camera cam;

	// Random number generation to be used for antialiasing; 
	// drand48() was used for random numbers in the original text but Windows did not support; works in Mac and Unix
	std::random_device rd;									// will be used to obtain a seed for random number engine
	std::mt19937 gen(rd());									// Mersenne_twister_engine seeded with rd()	; 				
	std::uniform_real_distribution<float>dis(0.0f, 1.0f);		// uniform distribution between 0<= x <1

	for (int j = ny - 1; j >= 0; --j)
	{
		for (int i = 0; i < nx; ++i)
		{
			Vec3 col(0.0f, 0.0f, 0.0f);
			for(std::size_t s{0}; s<ns; ++s)					// send several(ns) random rays within each pixel 
			{
				float u = float(i+dis(gen)) / float(nx);	    // dis(gen); random uniform distribution of floating numbers; 
				float v = float(j+dis(gen)) / float(ny);		// between 1 and 0 gen() is the Mersenne twister engine for random number
				Ray r = cam.get_ray(u, v);						// send rays to get a sample within that pixel
				Vec3 p = r.point_at_parameter(2.0f);
				col += color(r, world, 0);							// get the color if there is any hit
			}
			col  /= float(ns);									// use tge average of sample colors for a more smooth edge coloring; antialiasing
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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

