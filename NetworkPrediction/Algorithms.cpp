//////////////////////////////////////////////
//要添加的算法：
//一个朋友概率贡献，两个节点1-（1-p）^n
//1. 人数按聚集系数的分布
//2. 有一定共同朋友数的节点对数&其中是朋友的数量&概率
//3. 2.中“共同朋友”=“至少一方访问另一方两次非0的人”
//4. 2.中“共同朋友”=“两个人互相访问”
//5. 人数按入度，出度的分布
//

#include "Algorithms.h"

using namespace std;

void Algorithms::count_same_friends(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (i != k && k != j
			&& ( data[i][k].num || data[k][i].num )
			&& ( data[k][j].num || data[j][k].num ))
			weight_map[i][j] += 1;
	}
}

void Algorithms::multiply_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += sqrt(sqrt(double(data[i][k].sum * data[k][i].sum) * double(data[k][j].sum * data[j][k].sum)));
		}
	}
}

void Algorithms::multiply_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += sqrt(sqrt(double(data[i][k].num * data[k][i].num) * double(data[k][j].num * data[j][k].num)));
		}
	}
}

void Algorithms::add_four_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += (data[i][k].sum + data[k][i].sum + data[k][j].sum + data[j][k].sum);
		}
	}
	weight_map[i][j] /= 4;
}

void Algorithms::add_four_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += ( data[i][k].num + data[k][i].num + data[k][j].num + data[j][k].num );
		}
	}
	weight_map[i][j] /= 4;
}

void Algorithms::multiply_two_sums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += sqrt(double(data[i][k].sum + data[k][i].sum) * double(data[k][j].sum + data[j][k].sum));
		}
	}
	weight_map[i][j] /= 2;
}

void Algorithms::multiply_two_nums(network_data& data, double*const*const weight_map, unsigned i, unsigned j) {
	weight_map[i][j] = 0;
	for (unsigned k = 0; k < data.num_of_people; k++) {
		if (j != k && k != i) {
			weight_map[i][j] += sqrt(double(data[i][k].num + data[k][i].num) * double(data[k][j].num + data[j][k].num));
		}
	}
	weight_map[i][j] /= 2;
}

vector<item> Algorithms::find_count_same_friends(network_data& data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, count_same_friends, gen_no_direction);
}

vector<item> Algorithms::find_multiply_four_sums(network_data& data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, multiply_four_sums, gen_no_direction);
}

std::vector<item> Algorithms::find_multiply_four_nums(network_data & data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, multiply_four_nums, gen_no_direction);
}

std::vector<item> Algorithms::find_add_four_sums(network_data & data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, add_four_sums, gen_no_direction);
}

std::vector<item> Algorithms::find_add_four_nums(network_data & data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, add_four_nums, gen_no_direction);
}

std::vector<item> Algorithms::find_multiply_two_sums(network_data & data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, multiply_two_sums, gen_no_direction);
}

std::vector<item> Algorithms::find_multiply_two_nums(network_data & data) {
	return use_func_to_calc_and_sort(data, loop_no_direction, multiply_two_nums, gen_no_direction);
}


















































///////////////////////////////////////////////////////////////////////
//Following are function apis to use as shared code
std::vector<item> Algorithms::use_func_to_calc_and_sort(
	network_data& data,
	void(*calc_loop)(network_data&, double*const*const, void(*)( network_data&, double*const*const, unsigned, unsigned)),
	void(*calc_func)(network_data&, double*const*const, unsigned, unsigned),
	std::vector<item>(*gen_func)(network_data&, double*const*const )) {
	double** const weight_map = new double*[data.num_of_people];
	double* const map = new double[data.num_of_people*data.num_of_people]{};
	for (size_t i = 0; i < data.num_of_people; i++) {
		weight_map[i] = map + data.num_of_people*i;
	}
	calc_loop(data, weight_map, calc_func);
	std::vector<item> rtn = gen_func(data, weight_map);
	std::sort(rtn.begin(), rtn.end(), comp_weight);
	delete[] weight_map;
	delete[] map;
	return rtn;
}

void Algorithms::loop_no_direction(
	network_data& data, 
	double*const*const weight_map,
	void(*calc_func)( network_data&, double*const*const, unsigned, unsigned )) {
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = i + 1; j < data.num_of_people; j++) {
			if (!data[i][j].num && !data[j][i].num) {
				calc_func(data, weight_map, i, j);
			}
		}
	}
}

void Algorithms::loop_all(
	network_data& data,
	double*const*const weight_map,
	void(*calc_func)( network_data&, double*const*const, unsigned, unsigned )) {
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			if (i!=j && !data[i][j].num) {
				calc_func(data, weight_map, i, j);
			}
		}
	}
}

vector<item> Algorithms::gen_no_direction(network_data& data, double*const*const weight_map) {
	std::vector<item> rtn;
	rtn.reserve(data.num_of_people*( data.num_of_people + 1 ) / 2);
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = i + 1; j < data.num_of_people; j++) {
			if (weight_map[i][j] != 0) {
				rtn.emplace_back(item{ data.people[i], data.people[j], weight_map[i][j], &data[i][j] });
			}
		}
	}
	rtn.shrink_to_fit();
	return rtn;
}

std::vector<item> Algorithms::gen_all(network_data & data, double*const*const weight_map) {
	std::vector<item> rtn;
	rtn.reserve(data.num_of_people*data.num_of_people);
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			if (weight_map[i][j] != 0) {
				rtn.emplace_back(item{ data.people[i], data.people[j], weight_map[i][j],&data[i][j] });
			}
		}
	}
	rtn.shrink_to_fit();
	return rtn;
}

bool Algorithms::comp_weight(const item& it1, const item& it2) {
	return it1.weight >= it2.weight;
}

////////////////////////////////////////////////////////////////////////////////////
//Following code is used for calculating clustering coefficients and its ditribution
bool compare_cl_coeff(const clustering& val1, const clustering& val2) {
	if (isnan(val1.cl_coeff) && isnan(val2.cl_coeff)) return val1.person < val2.person;
	if (isnan(val1.cl_coeff)) return false;
	if (isnan(val2.cl_coeff)) return true;
	if (val1.cl_coeff == val2.cl_coeff) return val1.person < val2.person;
	return( val1.cl_coeff < val2.cl_coeff );
}

std::vector<clustering> Algorithms::find_clustering_coeff(network_data & data) {
	std::vector<clustering> rtn(data.num_of_people);
	for (unsigned i = 0; i < data.num_of_people; i++) {
		int neighbor_count = 0;
		int neighbor_edge_count = 0;
		for (unsigned j = 0; j < data.num_of_people; j++) {
			if (data[i][j].num || data[j][i].num) {
				neighbor_count++;
				for (unsigned k = j + 1; k < data.num_of_people; k++) {
					if (( data[i][k].num || data[k][i].num ) && ( data[j][k].num || data[k][j].num )) {
						neighbor_edge_count++;
					}
				}
			}
		}
		rtn[i].cl_coeff = 2.0 * neighbor_edge_count / ( neighbor_count*( neighbor_count - 1 ) );
		rtn[i].person = data.people[i];
	}
	sort(rtn.begin(), rtn.end(), compare_cl_coeff);
	return rtn;
}

std::array<int, 12> Algorithms::find_clust_distrib(std::vector<clustering>& data) {
	std::array<int, 12> rtn{};
	size_t size = data.size();
	for (size_t i = 0; i < size; i++) {
		if (data[i].cl_coeff == 0) { rtn[0]++; continue; }
		if (isnan(data[i].cl_coeff)) { rtn[11]++; continue; }
		unsigned cap = 1;
		for (; cap < 11 && cap / 10.0 < data[i].cl_coeff; cap++);
		rtn[cap]++;
	}
	return rtn;
}


//////////////////////////////////////////////////////////////////////////////////
//Following code is used for calculating the same_friend-friend_probability list
std::vector<std::vector<int>> Algorithms::find_num_of_same_friend(network_data& data) {
	std::vector<std::vector<int>> rtn(data.num_of_people);
	for (unsigned i = 0; i < data.num_of_people; i++) {
		rtn[i].resize(data.num_of_people);
	}
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			rtn[i][j] = 0;
			for (unsigned k = 0; k < data.num_of_people; k++) {
				if (i != j && i != k && k != j
					&& ( data[i][k].num || data[k][i].num )
					&& ( data[j][k].num || data[k][j].num ))
					rtn[i][j]++;
			}
		}
	}
	return rtn;
}

std::vector<unsigned> Algorithms::count_friends_and_trios(network_data& data, double*const*const weight_map) {
	loop_no_direction(data, weight_map, count_same_friends);
	std::vector<unsigned> rtn;
	double max_same = 0;
	//scope for same_friends
	{
//		std::vector<std::vector<int>> same_friends = find_num_of_same_friend(data);
		for (unsigned i = 0; i < data.num_of_people; i++) {
			for (unsigned j = i + 1; j < data.num_of_people; j++) {
				max_same = std::max(max_same, weight_map[i][j]);
			}
		}
		rtn.resize(size_t( max_same + 1 ) * 2);
		for (unsigned i = 0; i < data.num_of_people; i++) {
			for (unsigned j = i + 1; j < data.num_of_people; j++) {
				rtn[2 * size_t(weight_map[i][j])]++;
				if (data[i][j].num || data[j][i].num) rtn[2 * size_t(weight_map[i][j]) + 1]++;
			}
		}
	}
	return rtn;
}







