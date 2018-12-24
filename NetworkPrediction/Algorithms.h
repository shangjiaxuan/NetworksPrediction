#pragma once

#include "Relation_array.h"

struct set_info {
//	set_info() = delete;
	const std::vector<int>* people;
	const network_data* source;
	int group = 0;
};

struct clustering {
	int person = -1;
	double cl_coeff = 0;
};

class Algorithms {
public:
	static std::vector<network_data> separate_sets(network_data& source);
	//put your algorithms here:
	static std::vector<item> func1(const network_data& data);
//	static sorted_items func2(const network_data& data);
	static std::vector<clustering> find_clustering_coeff(network_data& data);
	//index 0 gives number of 0; 11 gives nan
	static std::array<int, 12> find_clust_distrib(std::vector<clustering>& data);
private:
	static network_data find_one_subset(set_info info);
	static network_data find_one_subset_helper(set_info& info);
	static void find_one_subset_loop(std::vector<char>& cur_group, std::vector<int>& found, const network_data& source, int cur, int start, int group);
};
