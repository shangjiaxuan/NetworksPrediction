#include "Algorithms.h"
#include "ThreadManager.h"
#include "IO_Manager.h"

using namespace std;

void Algorithms::find_one_subset_loop(vector<char>& cur_group,
									vector<int>& found,
									const network_data& source,
									int cur,
									int start,
									int group) {
	for(unsigned i = start; i < source.num_of_people; i++) {
		if(!found[i]) {
			if(source[cur][i].num || source[i][cur].num) {
				found[i] = group;
				cur_group[i] = true;
				find_one_subset_loop(cur_group, found, source, i, start, group);
			}
		}
	}
}

network_data Algorithms::find_one_subset(set_info info) {
	network_data rtn;
	rtn.max_index = 0;
	for(unsigned i = 0; i < info.source->num_of_people; i++) {
		if((*(info.people))[i] == info.group) {
			rtn.num_of_people++;
			if(info.source->people[i] > rtn.max_index) rtn.max_index = info.source->people[i];
		}
	}
	rtn.index = new unsigned[rtn.max_index + 1];
	rtn.people = new int[rtn.num_of_people];
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people]();
	int cur_index = 0;
	for(unsigned i = 0; i < info.source->num_of_people && info.source->people[i] <= rtn.max_index; i++) {
		if((*(info.people))[i] == info.group) {
			rtn.people[cur_index] = info.source->people[i];
			rtn.index[info.source->people[i]] = cur_index;
			cur_index++;
		} else {
			rtn.index[info.source->people[i]] = -1;
		}
	}
	int i_rtn = 0;
	for(unsigned i = 0; i < info.source->num_of_people; i++) {
		if((*(info.people))[i] == info.group) {
			int j_rtn = 0;
			for(unsigned j = 0; j < info.source->num_of_people; j++) {
				if((*(info.people))[j] == info.group) {
					rtn[i_rtn][j_rtn] = (*(info.source))[i][j];
					j_rtn++;
				}
			}
			i_rtn++;
		}
	}
	for(unsigned i = 0; i < rtn.num_of_people; i++) {
		for(unsigned j = 0; j < rtn.num_of_people; j++) {
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

bool comp_rel_num(const network_data& i, const network_data& j) {
	return i.num_of_directional_edge < j.num_of_directional_edge;
}

network_data Algorithms::find_one_subset_helper(set_info& info) {
	return Algorithms::find_one_subset(info);
}

vector<network_data> Algorithms::separate_sets(network_data& source) {
	vector<int> found(source.num_of_people);
	vector<char> cur_group(source.num_of_people);
	int group_index = 0;
	for(unsigned i = 0; i < source.num_of_people; i++) {
		if(!found[i]) {
			group_index++;
			for(unsigned k = 0; k < source.num_of_people; k++) {
				cur_group[k] = false;
			}
			cur_group[i] = true;
			found[i] = group_index;
			//one loop of sorting things into one group
			find_one_subset_loop(cur_group, found, source, i, i, group_index);
		}
	}
	vector<set_info> passed(group_index);
	for(int i=0; i<group_index; i++) {
		passed[i] = set_info{ &found, &source,i + 1 };
	}
	vector<network_data> rtn = Thread_Manager<set_info, network_data>::vector_async_copy(passed, find_one_subset);
//	network_data::destroy(source);
	std::sort(rtn.begin(), rtn.end(), comp_rel_num);
	return rtn;
}
