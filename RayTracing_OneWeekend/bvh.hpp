//#pragma once
#ifndef BVH_H
#define BVH_H
#include "Hitable.hpp"
#include <random>
//#include <stdlib.h>
//#include <cstdlib>

class bvh_node : public Hitable
{
public:
	bvh_node() :left{nullptr}, right{nullptr}, box{} { }
	bvh_node(Hitable** l, int n, float time0, float time1);
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;
	Hitable* left;
	Hitable* right;
	aabb box;
};

bool bvh_node::bounding_box(float t0, float t1, aabb& b) const
{
	b = box;
	return true;
}

bool bvh_node::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	if (box.hit(r, t_min, t_max))
	{
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right)
		{
			if (left_rec.t < right_rec.t)
			{
				rec = left_rec;
				return true;
			}
			else
			{
				rec = right_rec;
				return true;
			}
		}
		else if (hit_left)
		{
			rec = left_rec;
			return true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
		else
			return false;
	}
	return false;
}

int box_x_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))  // check whether there is a bounding box for an infinite plane that does not have a bounding box; not used here
	{
		std::cerr << "\nNo bounding box in bvh_node constructor\n";
	}
	//delete ah, bh;
	if (box_left.min().x() - box_right.min().x()<0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int box_y_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable**)a;  
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))  // check whether there is a bounding box for an infinite plane that does not have a bounding box; not used here
	{
		std::cerr << "\nNo bounding box in bvh_node constructor\n";
	}
	//delete ah, bh;
	if (box_left.min().y() - box_right.min().y() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int box_z_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable * *)a;
	Hitable* bh = *(Hitable * *)b;

	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))  // check whether there is a bounding box for an infinite plane that does not have a bounding box; not used here
	{
		std::cerr << "\nNo bounding box in bvh_node constructor\n";
	}
	//delete ah, bh;
	if (box_left.min().z() - box_right.min().z() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}

}

bvh_node::bvh_node(Hitable** l, int n, float time0, float time1)
{
	std::random_device rd;									// will be used to obtain a seed for random number engine
	std::mt19937 gen(rd());									// mersenne_twister_engine seeded with rd()	; 				
	std::uniform_real_distribution<float>dis(0.0f, 1.0f);
	
	int axis = int(3 * dis(gen));							// choose a random axis to sort
	if (axis == 0)
	{
		qsort(l, n, sizeof(Hitable*), box_x_compare);
	}
	else if(axis ==1)
	{
		qsort(l, n, sizeof(Hitable*), box_y_compare);
	}
	else
	{
		qsort(l, n, sizeof(Hitable*), box_z_compare);
	}
	if (n == 1)														// end recursion if n==1
	{
		left = right = l[0];
	}
	else if(n==2)													// end recursion if n==2	
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new bvh_node(l, n / 2, time0, time1);				// divide the list to half and continue recursion until there is one or two elements in the hitable_list				
		right = new bvh_node(l + n / 2, n - n / 2, time0, time1);
	}
	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
		std::cerr << "\nNo bounding box in bvh_node constructor\n";
	box = surrounding_box(box_left, box_right);
}


#endif // !BVH_H
