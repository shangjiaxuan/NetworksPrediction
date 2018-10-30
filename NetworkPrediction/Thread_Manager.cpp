#include "Thread_Manager.h"

using namespace std;

sorted_items launch_function(bool* lock, sort_rel_func function, network_data* data) {
	*lock = true;
	const sorted_items& rtn = function(data);
	*lock = false;
	return rtn;
}

set_of_sorted Thread_Manager::work(const data_sets& source, sort_rel_func function) {
	const unsigned num_of_core = thread::hardware_concurrency();
	set_of_sorted rtn;
	rtn.num = source.num;
	rtn.data = new sorted_items[source.num];
	future<sorted_items>* buffer = new future<sorted_items>[source.num];
	unsigned* tasks = new unsigned[num_of_core];
	bool* locks = new bool[source.num]();
	for(unsigned i = 0; i < source.num;) {
		for(unsigned j = 1; j < num_of_core; j++) {
			if(!locks[tasks[j]]) {
				buffer[i] = std::async(std::launch::async, launch_function, &locks[i], function, &source.pdata[i]);
				tasks[j] = i;
				i++;
			}
		}
	}
	delete[] tasks;
	for(unsigned i = 0; i < source.num; i++) {
		rtn.data[i] = buffer[i].get();
	}
	delete[] locks;
	delete[] buffer;
	return rtn;
}
