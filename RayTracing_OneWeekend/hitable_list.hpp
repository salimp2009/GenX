//#pragma once
#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H
#include "Hitable.hpp"

class Hitable_list:public Hitable
{
public:
	Hitable_list():list{nullptr},list_size{0} { }
	Hitable_list(Hitable **l, int n):list{l}, list_size{n} { }
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	~Hitable_list() { delete list; }
	Hitable** list;
	int list_size;
};

bool Hitable_list::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything{ false };
	double closest_so_far{ t_max };
	for (int i{ 0 }; i < list_size; ++i)
	{
		if(list[i]->hit(r, t_min, closest_so_far, temp_rec)) 
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // !HITABLE_LIST_H

