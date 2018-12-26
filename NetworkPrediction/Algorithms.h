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
//For separating sets
	static std::vector<network_data> separate_sets_move(network_data& source);
//	static std::vector<network_data> separate_sets(network_data& source);

	//For clustering coefficients
	static std::vector<clustering> find_clustering_coeff(network_data& data);
	//index 0 gives number of 0; 11 gives NAN
	static std::array<int, 12> find_clust_distrib(std::vector<clustering>& data);
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
	static std::vector<item> find_count_twice_nzero_friend(network_data& data);
	static std::vector<item> find_special0(network_data& data);

protected:
	//functions to calculate weight for nodes
	static void count_same_friends_all(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void count_same_friends_twice_nzero(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void add_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void add_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_two_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void multiply_two_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	static void special_calc0(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
//	static void special_calc1(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
private:
	//Utility function to use when finding the result 
	typedef bool(*calc_condition_t)(network_data& data, unsigned i, unsigned j);
	typedef void(*calc_func_t)(network_data& data, double*const*const weight_map, unsigned i, unsigned j);
	typedef void(*calc_loop_t)( network_data& data, double*const*const weight_map, calc_func_t calc_func, calc_condition_t calc_condition);
	typedef std::vector<item>(*gen_func_t)(network_data& data, double*const*const weight_map);
	static std::vector<item> use_func_to_calc_and_sort(
		network_data& data,
		calc_condition_t calc_condition,
		calc_loop_t calc_loop,
		calc_func_t calc_func,
		gen_func_t gen_func);
	static bool calc_all(network_data& data, unsigned i, unsigned j) { return true; }
	static bool calc_none(network_data& data, unsigned i, unsigned j) { return false; }
	static bool calc_exclude_i2j(network_data& data, unsigned i, unsigned j) { return !data[i][j].num; }
//	static bool calc_exclude_j2i(network_data& data, unsigned i, unsigned j) { return !data[j][i].num; }
	static bool calc_ineqj(network_data& data, unsigned i, unsigned j) { return i != j; }
	static bool calc_no_any_friend(network_data& data, unsigned i, unsigned j) { return !( data[i][j].num || data[j][i].num ); }
	static bool calc_no_self_or_is_friend_one_dir(network_data& data, unsigned i, unsigned j) { return !data[i][j].num && i != j; }
	static void loop_no_direction(network_data& data, double*const*const weight_map, calc_func_t calc_func, calc_condition_t calc_condition);
	static void loop_all(network_data& data, double*const*const, calc_func_t calc_func, calc_condition_t calc_condition);
	static std::vector<item> gen_no_direction(network_data& data, double*const*const);
	static std::vector<item> gen_all(network_data& data, double*const*const);

//For calculating statistical data
public:
	static std::vector<unsigned> find_count_friends_all(network_data& data);
	static std::vector<unsigned> find_count_friends_least_twice_nzero(network_data& data);
	static std::vector<unsigned> find_count_mutual_visit(network_data& data);
protected:
	static void count_same_friends_all(network_data& data, size_t*const*const weight_map, unsigned i, unsigned j);
	static void count_same_friends_least_twice_nzero(network_data& data, size_t*const*const weight_map, unsigned i, unsigned j);
	static void count_mutual_visit(network_data& data, size_t*const*const weight_map, unsigned i, unsigned j);

private:
	typedef void(*count_func_t)(network_data& data, size_t*const*const num_map, unsigned i, unsigned j);
	static std::vector<unsigned> count_friends_host(network_data& data, count_func_t count_func);


public:
//For separating sets
	static network_data find_one_subset_move(set_info& info);
//	static network_data find_one_subset(set_info& info);
//	static void find_one_subset_loop(std::vector<char>& cur_group, std::vector<int>& found, const network_data& source, int cur, int start, int group);
	//For calculating the same_friend-friend_probability list
	static std::vector<std::vector<int>> find_num_of_same_friend(network_data& data);
};
