#include "Algorithms.h"
#include <immintrin.h>

using namespace std;

void Algorithms::find_one_subset_loop(bool* cur_group,
									int* found,
									network_data& source,
									int cur,
									int start,
									int group) {
	for(int i = start; i < source.num_of_people; i++) {
		if(!found[i]) {
			if(source[cur][i].num || source[i][cur].num) {
				found[i] = group;
				cur_group[i] = true;
				find_one_subset_loop(cur_group, found, source, i, start, group);
			}
		}
	}
}

network_data Algorithms::find_one_subset(int* people, network_data& source, int group) {
	network_data rtn;
	rtn.max_index = 0;
	for(int i = 0; i < source.num_of_people; i++) {
		if(people[i] == group) {
			rtn.num_of_people++;
			if(source.people[i] > rtn.max_index) rtn.max_index = source.people[i];
		}
	}
	rtn.index = new int[rtn.max_index + 1];
	rtn.people = new int[rtn.num_of_people];
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people]();
	int cur_index = 0;
	for(int i = 0; i < source.num_of_people && source.people[i] <= rtn.max_index; i++) {
		if(people[i] == group) {
			rtn.people[cur_index] = source.people[i];
			rtn.index[source.people[i]] = cur_index;
			cur_index++;
		} else {
			rtn.index[source.people[i]] = -1;
		}
	}
	int i_rtn = 0;
	for(int i = 0; i < source.num_of_people; i++) {
		if(people[i] == group) {
			int j_rtn = 0;
			for(int j = 0; j < source.num_of_people; j++) {
				if(people[j] == group) {
					rtn[i_rtn][j_rtn] = source[i][j];
					j_rtn++;
				}
			}
			i_rtn++;
		}
	}
	for(int i = 0; i < rtn.num_of_people; i++) {
		for(int j = 0; j < rtn.num_of_people; j++) {
			if(rtn[i][j].num) {
				sort(rtn[i][j].data, rtn[i][j].data + rtn[i][j].num);
				rtn.num_of_directional_edge++;
				if(rtn[j][i].num) rtn.num_of_non_directional--;
				rtn.num_of_records += rtn[i][j].num;
				rtn.sum_of_records += rtn[i][j].sum;
			}
		}
	}
	rtn.num_of_non_directional >>= 1;
	rtn.num_of_non_directional += rtn.num_of_directional_edge;
	return rtn;
}

bool comp_num_of_people(const network_data& i, const network_data& j) {
	return i.num_of_people < j.num_of_people;
}

data_sets Algorithms::separate_sets(network_data& source) {
	int* found = new int[source.num_of_people]();
	bool* cur_group = new bool[source.num_of_people];
	int top = -1;
	int group_index = 0;
	for(int i = 0; i < source.num_of_people; i++) {
		if(!found[i]) {
			group_index++;
			for(int k = 0; k < source.num_of_people; k++) {
				cur_group[k] = false;
			}
			cur_group[i] = true;
			found[i] = group_index;
			//one loop of sorting things into one group
			find_one_subset_loop(cur_group, found, source, i, i, group_index);
		}
	}
	data_sets rtn;
	rtn.num = group_index;
	rtn.pdata = new network_data[group_index];
	for(int i = 0; i < group_index; i++) {
		rtn.pdata[i] = find_one_subset(found, source, i + 1);
	}
	delete[] found;
	delete[] cur_group;
	network_data::destroy(source);
	std::sort(rtn.pdata, rtn.pdata + rtn.num, comp_num_of_people);
	return rtn;
}
