#pragma once

#include "Relation_array.h"

template<typename input, typename output>
class Thread_Manager {
public:
	//Probably change the implemenation so that it doesn't use an array of std::future, but a array of return values (and an array of booleans, as it is now). This may need to be rewritten using thread itself instead of async.
	static counted_array<output> work(const counted_array<input>& source, output(*function)(const input&));
//	static std::vector<output> work_vector(const std::vector<input>& source, output(*function)(const input&));
private:
	static output launch_function(bool* lock, output(*function)(const input&), input* data);
};

/*
template<typename input, typename output>
std::vector<output>
Thread_Manager<input, output>::work_vector(const std::vector<input>& source, output(*function)(const input&)) {
	const unsigned num_of_threads = std::thread::hardware_concurrency() - 1;
	std::vector<output> rtn;
	rtn.resize(source.size());
	std::vector<std::future<output>> buffer;
	buffer.resize(source.size());
	std::vector<unsigned> tasks;
	tasks.resize(num_of_threads);
	std::vector<bool> locks;
	locks.resize(num_of_threads);
	for (unsigned i = 0; i < num_of_threads; i++) {
		buffer[i] = std::async(std::launch::async, launch_function, locks[i], function, &source[i]);
		tasks[i] = i;
	}
	for (unsigned i = num_of_threads; i < source.size();) {
		for (unsigned j = 0; j < num_of_threads && i < source.size(); j++) {
			if (!locks[j]) {
				rtn[tasks[j]] = buffer[tasks[j]].get();
				buffer[i] = std::async(std::launch::async, launch_function, locks[j], function, &source[i]);
				tasks[j] = i;
				i++;
			}
		}
	}
	for (unsigned j = 0; j < num_of_threads; j++) {
		rtn[tasks[j]] = buffer[tasks[j]].get();
	}
	return rtn;
}
*/



//要改的内容：如果数组元素数量小于并发数-1。。。
template<typename input, typename output>
counted_array<output>
Thread_Manager<input, output>::work(const counted_array<input>& source, output(*function)(const input&)) {
	const unsigned num_of_threads = std::thread::hardware_concurrency() - 1;
	counted_array<output> rtn;
	rtn.num = source.num;
	rtn.data = new output[source.num];
	std::future<output>* buffer = new std::future<output>[source.num];
	unsigned* tasks = new unsigned[num_of_threads];
	bool* locks = new bool[num_of_threads]();
	for (unsigned i = 0; i < num_of_threads; i++) {
		buffer[i] = std::async(std::launch::async, launch_function, &locks[i], function, &source.data[i]);
		tasks[i] = i;
	}
	for (unsigned i = num_of_threads; i < source.num;) {
		for (unsigned j = 0; j < num_of_threads && i < source.num; j++) {
			if (!locks[j]) {
				rtn.data[tasks[j]] = buffer[tasks[j]].get();
				buffer[i] = std::async(std::launch::async, launch_function, &locks[j], function, &source.data[i]);
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
