#include "Algorithms.h"
#include "ThreadManager.h"
#include "IO_Manager.h"

//#include "Utilities.h"

using namespace std;

bool comp_num_of_people(const network_data& i, const network_data& j) {
	return i.num_of_people < j.num_of_people;
}

bool comp_rel_num(const network_data& i, const network_data& j) {
	return i.num_of_directional_edge < j.num_of_directional_edge;
}

vector<network_data> Algorithms::separate_sets_move(network_data& source) {
//	Timer<> time{};
	vector<int> found(source.num_of_people);
	vector<char> cur_group(source.num_of_people);
	int group_index = 0;
	for (unsigned i = 0; i < source.num_of_people; i++) {
		if (!found[i]) {
//			time.reset();
			group_index++;
			for (unsigned k = 0; k < source.num_of_people; k++) {
				cur_group[k] = false;
			}
			cur_group[i] = true;
			found[i] = group_index;
			//one loop of sorting things into one group
			{
				int* cur_stack = new int[source.num_of_people];
				int* loop_stack = new int[source.num_of_people];
				int stack_index = -1;
				int start = i;
				int cur = i;
				while(true) {
					for (unsigned j = start; j < source.num_of_people; j++) {
						if (!found[j] && ( source[cur][j].num || source[j][cur].num )) {
							found[j] = group_index;
							cur_group[j] = true;
							stack_index++;
							cur_stack[stack_index] = cur;
							loop_stack[stack_index] = j;
							stack_index++;
							cur_stack[stack_index] = j;
							loop_stack[stack_index] = i;
							break;
						}
					}
					if (stack_index<0) break;
					start = loop_stack[stack_index];
					cur = cur_stack[stack_index];
					stack_index--;
				}
				delete[] cur_stack;
				delete[] loop_stack;
			}
//			std::cout << "Time of current loop:\t" << time.elapsed() << std::endl;
		}
	}
//	std::cout << "Time preparing for separation:\t" << time.elapsed() << std::endl;
//	time.reset();
	vector<set_info> passed;//(group_index);
	passed.reserve(group_index);
	for (int i = 0; i < group_index; i++) {
		passed.push_back({ found, source,i + 1 });
	}
//	std::cout << "Time preparing for parsing passed val:\t" << time.elapsed() << std::endl;
	vector<network_data> rtn = Thread_Manager::vector_thread<set_info,network_data>(passed, find_one_subset_move);
	network_data::destroy(source);
	std::sort(rtn.rbegin(), rtn.rend(), comp_num_of_people);
	return rtn;
}

network_data Algorithms::find_one_subset_move(set_info& info) {
	network_data rtn;
	rtn.max_index = 0;
//	Timer<> time;
	for (unsigned i = 0; i < info.source.num_of_people; i++) {
		if (info.people[i] == info.group) {
			rtn.num_of_people++;
			if (info.source.people[i] > rtn.max_index) rtn.max_index = info.source.people[i];
		}
	}
//	cout << "Time used finding subset max:\t" << time.elapsed() << endl;
//	time.reset();
	rtn.index = new unsigned[rtn.max_index + 1];
	rtn.people = new int[rtn.num_of_people];
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people]{};
//	cout << "Time used allocating memory:\t" << time.elapsed() << endl;
//	time.reset();
	int i_rtn = 0;
	for (unsigned i = 0; i < info.source.num_of_people && info.source.people[i] <= rtn.max_index; i++) {
		if (info.people[i] == info.group) {
			rtn.people[i_rtn] = info.source.people[i];
			rtn.index[info.source.people[i]] = i_rtn;
			int j_rtn = 0;
			for (unsigned j = 0; j < info.source.num_of_people; j++) {
				if ( info.people[j] == info.group) {
					rtn[i_rtn][j_rtn].num = info.source[i][j].num;
					rtn[i_rtn][j_rtn].sum = info.source[i][j].sum;
					rtn[i_rtn][j_rtn].data = info.source[i][j].data;
					info.source[i][j].data = nullptr;
					std::sort(rtn[i_rtn][j_rtn].data, 
							  rtn[i_rtn][j_rtn].data + rtn[i_rtn][j_rtn].num);
					j_rtn++;
				}
			}
			i_rtn++;
		} else {
			rtn.index[info.source.people[i]] = -1;
		}
	}
//	cout << "Time used syncing people, index and lists:\t" << time.elapsed() << endl;
//	time.reset();
	for (unsigned i = 0; i < rtn.num_of_people; i++) {
		for (unsigned j = 0; j < rtn.num_of_people; j++) {
			if (rtn[i][j].num) {
				rtn.num_of_directional_edge++;
				if (rtn[j][i].num) rtn.num_of_non_directional--;
				rtn.num_of_records += rtn[i][j].num;
				rtn.sum_of_records += rtn[i][j].sum;
			}
		}
	}
	rtn.num_of_non_directional >>= 1;
	rtn.num_of_non_directional += rtn.num_of_directional_edge;
//	cout << "Time used calculating properties:\t" << time.elapsed() << endl;
	return rtn;
}


























/*


void find_one_subset_loop_test(vector<char>& cur_group,
							   vector<int>& found,
							   const network_data& source,
							   int cur,
							   int start,
							   int group) {
	std::stack<int> stack_cur;
	std::stack<int> stack_loop;
	stack_cur.push(cur);
	stack_loop.push(start);
	while (!stack_loop.empty()) {
		int cur_start = stack_loop.top();
		stack_loop.pop();
		int cur_cur = stack_cur.top();
		stack_cur.pop();
		for (unsigned i = cur_start; i < source.num_of_people; i++) {
			if (!found[i] && ( source[stack_cur.top()][i].num || source[i][stack_cur.top()].num )) {
				found[i] = group;
				cur_group[i] = true;
				stack_cur.push(i);
				stack_loop.push(i);
				break;
			}
//			stack_cur.pop();
		}
//		stack_loop.pop();
	}
}

void Algorithms::find_one_subset_loop(vector<char>& cur_group,
									vector<int>& found,
									const network_data& source,
									int cur,
									int start,
									int group) {
	for(unsigned i = start; i < source.num_of_people; i++) {
		if(!found[i]&&( source[cur][i].num || source[i][cur].num )) {
			found[i] = group;
			cur_group[i] = true;
			find_one_subset_loop(cur_group, found, source, i, start, group);
		}
	}
}



vector<network_data> Algorithms::separate_sets(network_data& source) {
	Timer<> time{};
	vector<int> found(source.num_of_people);
	vector<char> cur_group(source.num_of_people);
	int group_index = 0;
	for (unsigned i = 0; i < source.num_of_people; i++) {
		if (!found[i]) {
			group_index++;
			for (unsigned k = 0; k < source.num_of_people; k++) {
				cur_group[k] = false;
			}
			cur_group[i] = true;
			found[i] = group_index;
			//one loop of sorting things into one group
			find_one_subset_loop(cur_group, found, source, i, i, group_index);
		}
	}
	vector<set_info> passed;
	passed.reserve(group_index);
	for (int i = 0; i < group_index; i++) {
		passed.push_back({ found, source,i + 1 });
	}
	std::cout << "Time preparing for separation:\t" << time.elapsed() << std::endl;
	vector<network_data> rtn = Thread_Manager<set_info, network_data>::vector_thread(passed, find_one_subset);
//	network_data::destroy(source);
	std::sort(rtn.begin(), rtn.end(), comp_rel_num);
	return rtn;
}



network_data Algorithms::find_one_subset(set_info& info) {
	network_data rtn;
	rtn.max_index = 0;
	Timer<> time;
	for (unsigned i = 0; i < info.source.num_of_people; i++) {
		if (info.people[i] == info.group) {
			rtn.num_of_people++;
			if (info.source.people[i] > rtn.max_index) rtn.max_index = info.source.people[i];
		}
	}
	cout << "Time used finding subset max:\t" << time.elapsed() << endl;
	time.reset();
	rtn.index = new unsigned[rtn.max_index + 1];
	rtn.people = new int[rtn.num_of_people];
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people]();
	cout << "Time used allocating memory:\t" << time.elapsed() << endl;
	time.reset();
	int cur_index = 0;
	for (unsigned i = 0; i < info.source.num_of_people && info.source.people[i] <= rtn.max_index; i++) {
		if (info.people[i] == info.group) {
			rtn.people[cur_index] = info.source.people[i];
			rtn.index[info.source.people[i]] = cur_index;
			cur_index++;
		} else {
			rtn.index[info.source.people[i]] = -1;
		}
	}
	cout << "Time used syncing people and index:\t" << time.elapsed() << endl;
	time.reset();
	int i_rtn = 0;
	for (unsigned i = 0; i < info.source.num_of_people; i++) {
		if (info.people[i] == info.group) {
			int j_rtn = 0;
			for (unsigned j = 0; j < info.source.num_of_people; j++) {
				if (info.people[j] == info.group) {
					rtn[i_rtn][j_rtn] = info.source[i][j];
					j_rtn++;
				}
			}
			i_rtn++;
		}
	}
	cout << "Time used copying lists:\t" << time.elapsed() << endl;
	time.reset();
	for (unsigned i = 0; i < rtn.num_of_people; i++) {
		for (unsigned j = 0; j < rtn.num_of_people; j++) {
			if (rtn[i][j].num) {
				sort(rtn[i][j].data, rtn[i][j].data + rtn[i][j].num);
				rtn.num_of_directional_edge++;
				if (rtn[j][i].num) rtn.num_of_non_directional--;
				rtn.num_of_records += rtn[i][j].num;
				rtn.sum_of_records += rtn[i][j].sum;
			}
		}
	}
	rtn.num_of_non_directional >>= 1;
	rtn.num_of_non_directional += rtn.num_of_directional_edge;
	cout << "Time used calculating properties:\t" << time.elapsed() << endl;
	return rtn;
}

*/
