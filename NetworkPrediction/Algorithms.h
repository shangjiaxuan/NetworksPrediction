#pragma once

#include "Relation_array.h"

struct set_info {
//	set_info() = delete;
	const std::vector<int>& people;
	const network_data& source;
	int group = 0;
};

struct clustering {
	int person = -1;
	double cl_coeff = 0;
};

class Algorithms {
	//put your algorithms here:
public:
	//For loading the input
//	static std::vector<network_data> separate_sets(network_data& source);
	static std::vector<network_data> separate_sets_move(network_data& source);

	//Sample output function
	static bool comp_weight(const item& it1, const item& it2);
	static std::vector<item> find_using_pure_same_friends(network_data& data);
	static std::vector<item> find_using_sum(network_data& data);

	//For clustering coefficients
	static std::vector<clustering> find_clustering_coeff(network_data& data);
	//index 0 gives number of 0; 11 gives NAN
	static std::array<int, 12> find_clust_distrib(std::vector<clustering>& data);

	//For project-specific utilities:
	static std::vector<unsigned> count_friends_and_trios(network_data& data);

	static void count_same_friends(network_data& data);

private:
//	static network_data find_one_subset(set_info& info);
//	static void find_one_subset_loop(std::vector<char>& cur_group, std::vector<int>& found, const network_data& source, int cur, int start, int group);
	static network_data find_one_subset_move(set_info& info);

	static std::vector<std::vector<int>> find_num_of_same_friend(network_data& data);
};
