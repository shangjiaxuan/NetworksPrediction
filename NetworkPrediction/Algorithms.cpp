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

bool compare_cl_coeff(const clustering& val1, const clustering& val2) {
	if (isnan(val1.cl_coeff) && isnan(val2.cl_coeff)) return val1.person < val2.person;
	if (isnan(val1.cl_coeff)) return false;
	if (isnan(val2.cl_coeff)) return true;
	if (val1.cl_coeff == val2.cl_coeff) return val1.person < val2.person;
	return(val1.cl_coeff < val2.cl_coeff);
}

vector<clustering> Algorithms::find_clustering_coeff(network_data & data) {
	vector<clustering> rtn(data.num_of_people);
	for(unsigned i=0; i<data.num_of_people; i++) {
		int neighbor_count = 0;
		int neighbor_edge_count = 0;
		for(unsigned j=0; j<data.num_of_people; j++) {
			if (data[i][j].num || data[j][i].num) {
				neighbor_count++;
				for(unsigned k=j+1; k<data.num_of_people; k++) {
					if((data[i][k].num||data[k][i].num)&&(data[j][k].num||data[k][j].num)) {
						neighbor_edge_count++;
					}
				}
			}
		}
		rtn[i].cl_coeff = 2.0 * neighbor_edge_count / (neighbor_count*(neighbor_count - 1));
		rtn[i].person = data.people[i];
	}
	sort(rtn.begin(), rtn.end(), compare_cl_coeff);
	return rtn;
}

std::array<int, 12> Algorithms::find_clust_distrib(std::vector<clustering>& data) {
	std::array<int, 12> rtn{};
	size_t size = data.size();
	for ( size_t i = 0; i < size; i++ ) {
		if (data[i].cl_coeff == 0) { rtn[0]++; continue; }
		if (isnan(data[i].cl_coeff)) { rtn[11]++; continue; }
		unsigned cap = 1;
		for (; cap < 11 && cap / 10.0 < data[i].cl_coeff; cap++);
		rtn[cap]++;
	}
	return rtn;
}

std::vector<std::vector<int>> Algorithms::find_num_of_same_friend(network_data& data) {
	std::vector<std::vector<int>> rtn(data.num_of_people);
	for (unsigned i = 0; i < data.num_of_people; i++) {
		rtn[i].resize(data.num_of_people);
	}
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			rtn[i][j] = 0;
			for (unsigned k = 0; k < data.num_of_people; k++) {
				if (i!=j && i!=k && k!=j
					&&(data[i][k].num||data[k][i].num)
					&&(data[j][k].num||data[k][j].num))
					rtn[i][j]++;
			}
		}
	}
	return rtn;
}

std::vector<unsigned> Algorithms::count_friends_and_trios(network_data& data) {
	count_same_friends(data);
	std::vector<unsigned> rtn;
	unsigned max_same = 0;
	//scope for same_friends
	{
//		std::vector<std::vector<int>> same_friends = find_num_of_same_friend(data);
		for (unsigned i = 0; i < data.num_of_people; i++) {
			for (unsigned j = i + 1; j < data.num_of_people; j++) {
				max_same = std::max(max_same, data[i][j].same_friend);
			}
		}
		rtn.resize(( max_same + 1 ) * 2);
		for (unsigned i = 0; i < data.num_of_people; i++) {
			for (unsigned j = i + 1; j < data.num_of_people; j++) {
				rtn[2* data[i][j].same_friend]++;
				if (data[i][j].num || data[j][i].num) rtn[2* data[i][j].same_friend +1]++;
			}
		}
	}
	return rtn;
}

void Algorithms::count_same_friends(network_data& data) {
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			for (unsigned k = 0; k < data.num_of_people; k++) {
				if (i != j && i != k && k != j
					&& ( data[i][k].num || data[k][i].num )
					&& ( data[j][k].num || data[k][j].num ))
					data[i][j].same_friend++;
			}
		}
	}
}

bool Algorithms::comp_weight(const item& it1, const item& it2) {
	return it1.weight >= it2.weight;
}

vector<item> Algorithms::find_using_pure_same_friends(network_data& data) {
	vector<item> rtn;
	rtn.reserve( data.num_of_people*( data.num_of_people + 1 ) / 2 );
	size_t offset = 0;
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = i + 1; j < data.num_of_people; j++) {
			if (data[i][j].same_friend&&!(data[i][j].num||data[j][i].num)){
				rtn.emplace_back(item{ data.people[j], data.people[i],double(data[i][j].same_friend),&data[i][j] });
			}
		}
	}
	rtn.shrink_to_fit();
	std::sort(rtn.begin(), rtn.end(), comp_weight);
	return rtn;
}



