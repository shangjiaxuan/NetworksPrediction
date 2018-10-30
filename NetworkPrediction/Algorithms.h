#pragma once

#include "Relation_array.h"

struct set_info {
	set_info() = delete;
	int* people = nullptr;
	const network_data& source;
	int group = 0;
};

class Algorithms {
public:
	static data_sets separate_sets(const network_data& source);
	//put your algorithms here:
	static sorted_items func1(const network_data& data);
//	static sorted_items func2(const network_data& data);
private:
	static network_data find_one_subset(set_info info);
	static void find_one_subset_loop(bool* cur_group, int* found, const network_data& source, int cur, int start, int group);
	static network_data info_ptr_helper(set_info*const & info);
};
