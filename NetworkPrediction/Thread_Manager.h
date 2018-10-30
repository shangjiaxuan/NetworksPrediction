#pragma once

#include "Relation_array.h"

template<typename input, typename output>
class Thread_Manager {
public:
	static counted_array<output> work(const counted_array<input>& source, output(*function)(const input&));;
private:
	static output launch_function(bool* lock, output(*function)(const input&), input* data);
};









template<typename input, typename output>
counted_array<output>
Thread_Manager<input, output>::work(const counted_array<input>& source, output(*function)(const input&)) {
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
		for (unsigned j = 0; j < num_of_threads && i < source.num; j++) {
			if (!locks[tasks[j]]) {
				rtn.data[tasks[j]] = buffer[tasks[j]].get();
				buffer[i] = std::async(std::launch::async, launch_function, &locks[i], function, &source.data[i]);
				tasks[j] = i;
				i++;
			}
		}
	}
	for (unsigned j = 0; j < num_of_threads; j++) {
		rtn.data[tasks[j]] = buffer[tasks[j]].get();
	}
	delete[] tasks;
	delete[] locks;
	delete[] buffer;
	return rtn;
}

template<typename input, typename output>
output
Thread_Manager<input, output>::
launch_function(bool* lock, output(*function)(const input&), input* data) {
	*lock = true;
	const output& rtn = function(*data);
	*lock = false;
	return rtn;
}
