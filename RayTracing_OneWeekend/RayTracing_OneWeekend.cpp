#include <iostream>
#include <string>
#include <fstream>
#include "vec3.hpp"

// Ray Tracer Project from "RayTracing Over a Weekend" by Peter Shirley
// Sample PPM color chart from "RayTracing Over a Weekend" by Peter Shirley
// Vec3 not complete yet

int main()
{
	// test cases for Vec3
	//Vec3 points{ 1.1f,1.2f,1.3f};
	//Vec3 point2{3.0f, 4.0f, 5.0f};
	//std::cout << "\nPoints: ";
	//std::cout << points.x() << " " << points.y() << " " << points.z() << '\n';

	//Vec3 point3 = -point2;
	//std::cout << point3[0] << '\n';
	//point3[2] = 10;
	//std::cout << point3[2] << '\n';

	// testing graphic output as PPM file
	std::string file {"ppmexample.ppm"};
	std::ofstream ost(file);
	
	int nx{200};
	int ny{100};

	ost<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
	for(int j=ny-1; j>=0; --j)  {
	    for(int i=0; i<nx; ++i) {
	        //float r=float(i) / float(nx);
	        //float g=float(j) / float(ny);
	        //float b=0.2;
			Vec3 col{ float(i) / float(nx), float(j) / float(ny), 0.2f };
			
			int ir=int(255.99*col[0]);
	        int ig=int(255.99*col[1]);
	        int ib=int(255.99*col[2]);

	       // std::cout<<ir<<" "<<ig<<" "<<ib<<'\n';
	        ost<<ir<<" "<<ig<<" "<<ib<<"\n";
	    }
	}
	return 0;
}

