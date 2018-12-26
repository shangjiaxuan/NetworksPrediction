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
	static bool comp_weight(const item& it1, const item& it2);
	//output functions to use outside
	static std::vector<item> find_count_same_friends(network_data& data);
	static std::vector<item> find_multiply_four_sums(network_data& data);
	static std::vector<item> find_multiply_four_nums(network_data& data);
	static std::vector<item> find_add_four_sums(network_data& data);
	static std::vector<item> find_add_four_nums(network_data& data);
	static std::vector<item> find_multiply_two_sums(network_data& data);
	static std::vector<item> find_multiply_two_nums(network_data& data);
private:
	//functions to calculate weight for nodes
	static void count_same_friends(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void add_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void add_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_two_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_two_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);

public:
	//For separating sets
	static std::vector<network_data> separate_sets_move(network_data& source);
//	static std::vector<network_data> separate_sets(network_data& source);

	//For clustering coefficients
	static std::vector<clustering> find_clustering_coeff(network_data& data);
	//index 0 gives number of 0; 11 gives NAN
	static std::array<int, 12> find_clust_distrib(std::vector<clustering>& data);

	//For calculating the same_friend-friend_probability list
	static std::vector<unsigned> count_friends_and_trios(network_data& data);

private:
	//Utility function to use when finding the result 
	static std::vector<item> use_func_to_calc_and_sort(
		network_data& data,
		void(*calc_loop)( network_data&, double*const*const, void(*)( network_data&, double*const*const, unsigned, unsigned ) ),
		void(*calc_func)( network_data&, double*const*const, unsigned, unsigned ),
		std::vector<item>(*gen_func)( network_data&, double*const*const ));
	static void loop_no_direction(network_data& data, double*const*const, void(*calc_func)( network_data&, double*const*const, unsigned, unsigned ));
	static void loop_all(network_data& data, double*const*const, void(*calc_func)( network_data&, double*const*const, unsigned, unsigned ));
	static std::vector<item> gen_no_direction(network_data& data, double*const*const);
	static std::vector<item> gen_all(network_data& data, double*const*const);

	//For separating sets
	static network_data find_one_subset_move(set_info& info);
//	static network_data find_one_subset(set_info& info);
//	static void find_one_subset_loop(std::vector<char>& cur_group, std::vector<int>& found, const network_data& source, int cur, int start, int group);

public:
	//For calculating the same_friend-friend_probability list
	static std::vector<std::vector<int>> find_num_of_same_friend(network_data& data);
};
