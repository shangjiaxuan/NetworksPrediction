#pragma once

#include "Relation_array.h"

typedef sorted_items (*sort_rel_func)(const network_data&);

template<typename input, typename output>
class Thread_Manager {
public:
	typedef output(*func)(const input&);
	static counted_array<output> work(const counted_array<input>& source, func function) {
		const unsigned num_of_threads = std::thread::hardware_concurrency() - 1;
		counted_array<output> rtn;
		rtn.num = source.num;
		rtn.data = new output[source.num];
		std::future<output>* buffer = new std::future<output>[source.num];
		unsigned* tasks = new unsigned[num_of_threads];
		bool* locks = new bool[source.num]();
		for (unsigned i = 0; i < num_of_threads; i++) {
			buffer[i] = std::async(std::launch::async, launch_function, &locks[i], function, &source.data[i]);
			tasks[i] = i;
		}
		for (unsigned i = num_of_threads; i < source.num;) {
			for (unsigned j = 0; j < num_of_threads&&i < source.num; j++) {
				if (!locks[tasks[j]]) {
					buffer[i] = std::async(std::launch::async, launch_function, &locks[i], function, &source.data[i]);
					tasks[j] = i;
					i++;
				}
			}
		}
		delete[] tasks;
		for (unsigned i = 0; i < source.num; i++) {
			rtn.data[i] = buffer[i].get();
		}
		delete[] locks;
		delete[] buffer;
		return rtn;
	};
private:
	static output launch_function(bool* lock, func function, input* data) {
		*lock = true;
		const output& rtn = function(*data);
		*lock = false;
		return rtn;
	};
};

class find_results : Thread_Manager<network_data, sorted_items> {};

#include "Thread_Manager.ipp"
