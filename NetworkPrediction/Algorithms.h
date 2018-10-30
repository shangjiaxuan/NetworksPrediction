#pragma once

#include "Relation_array.h"

class Algorithms {
public:
	static data_sets separate_sets(network_data& source);
	//put your algorithms here:
	static sorted_items func1(const network_data* pdata);
	static sorted_items func2(const network_data* pdata);
private:
	static network_data find_one_subset(int* people, network_data& source, int group);
	static void find_one_subset_loop(bool* cur_group, int* found, network_data& source, int cur, int start, int group);
};
