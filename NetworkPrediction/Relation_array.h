#pragma once
#include "network_data.h"

struct item {
	int s_person = -1;
	int d_person = -1;
	double weight;
	list contact_list;
};

//template<typename node>
//struct counted_array {
//	node* data = nullptr;
//	unsigned num = 0;
//};

//typedef counted_array<network_data> data_sets;
//typedef counted_array<item> sorted_items;
//typedef counted_array<sorted_items> set_of_sorted;

