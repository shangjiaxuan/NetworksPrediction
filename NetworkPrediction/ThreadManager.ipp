#pragma once

template<typename input, typename output>
std::vector<output> Thread_Manager::vector_thread(std::vector<input>& source, output(*const function)(input&)) {
//	const unsigned subthreads = std::thread::hardware_concurrency() - 1;
	size_t size = source.size();
	std::vector<output> rtn(size);
	switch (int(workers)) {
	case -1:
		std::cout << "Cannot identify hardware concurrency, multithreading failed!\n";
	case 0:
		std::cout << "Only one thread supported!\n";
	case 1:
		std::cout << "Working as single thread!\n" << std::endl;
		for (size_t i = 0; i < size; i++) {
			rtn[i] = function(source[i]);
		}
		return rtn;
	default:
		bool* locks=new bool[workers];
		for (unsigned i = 0; i < ((workers < size) ? workers : size); i++) {
			std::thread t(launch_function<input,output>, &locks[i], function, &source[i], &rtn[i]);
			t.detach();
		}
		for (size_t i = workers; i < size;) {
			for (unsigned j = 0; j < workers && i < size; j++) {
				if (!locks[j]) {
					locks[j] = true;
					std::thread t(launch_function<input, output>, &locks[j], function, &source[i], &rtn[i]);
					t.detach();
					i++;
				}
			}
		}
		bool not_finished;
		do {
			not_finished = false;
			for (unsigned i = 0; i < workers; i++) {
				not_finished = not_finished || locks[i];
			}
		} while (not_finished);
		delete[] locks;
		return rtn;
	}
};

template<typename input, typename output>
void Thread_Manager::launch_function(bool* lock, output(*const function)(input&), input* data, output* out) {
	*out = function(*data);
	*lock = false;
};


template<typename input, typename output>
std::vector<output> Thread_Manager::vector_thread_copy(std::vector<input>& source, output(*const function)(input)) {
	size_t size = source.size();
	std::vector<output> rtn(size);
	switch (int(workers)) {
	case -1:
		std::cout << "Cannot identify hardware concurrency, multithreading failed!\n";
	case 0:
		std::cout << "Only one thread supported!\n";
	case 1:
		std::cout << "Working as single thread!\n" << std::endl;
		for (size_t i = 0; i < size; i++) {
			rtn[i] = function(source[i]);
		}
		return rtn;
	default:
		bool* locks = new bool[workers];
		for (unsigned i = 0; i < ((workers < size) ? workers : size); i++) {
			std::thread t(launch_function_copy<input, output>, &locks[i], function, &source[i], &rtn[i]);
			t.detach();
		}
		for (size_t i = workers; i < size;) {
			for (unsigned j = 0; j < workers && i < size; j++) {
				if (!locks[j]) {
					locks[j] = true;
					std::thread t(launch_function_copy<input, output>, &locks[j], function, &source[i], &rtn[i]);
					t.detach();
					i++;
				}
			}
		}
		bool not_finished;
		do {
			not_finished = false;
			for (unsigned i = 0; i < workers; i++) {
				not_finished = not_finished || locks[i];
			}
		} while (not_finished);
		delete[] locks;
		return rtn;
	}
};

template<typename input, typename output>
void Thread_Manager::launch_function_copy(bool* lock, output(*const function)(input), input* data, output* out) {
	*out = function(*data);
	*lock = false;
};

template<typename input, typename output>
std::vector<std::future<output>> Thread_Manager::vector_async_copy(std::vector<input>& source, output(*const function)( input )) {
	std::vector<std::future<output>> rtn(source.size());
	for (size_t i = 0; i < source.size(); i++) {
		rtn[i] = std::async(std::launch::async | std::launch::deferred, function, source[i]);
	}
	return rtn;
}

template<typename input, typename output>
std::vector<std::future<output>> Thread_Manager::vector_async(std::vector<input>& source, output(*const function)( input& )) {
	std::vector<std::future<output>> rtn(source.size());
	for (size_t i = 0; i < source.size(); i++) {
		rtn[i] = std::async(std::launch::async | std::launch::deferred, function, std::ref(source[i]));
	}
	return rtn;
}

template<typename input, typename output, size_t size>
std::array<std::future<output>, size> Thread_Manager::array_async(std::array<input, size>& source, output(*const function)( input& )) {
	std::array<std::future<output>,size> rtn;
	for (size_t i = 0; i < source.size(); i++) {
		rtn[i] = std::async(std::launch::async | std::launch::deferred, function, std::ref(source[i]));
	}
	return rtn;
}

template<typename input, typename output, size_t size>
std::array<std::future<output>, size> Thread_Manager::array_async_copy(std::array<input, size>& source, output(*const function)( input)) {
	std::array<std::future<output>, size> rtn;
	for (size_t i = 0; i < source.size(); i++) {
		rtn[i] = std::async(std::launch::async | std::launch::deferred, function, source[i]);
	}
	return rtn;
}








//use reverse order to access the light tasks that may have finished first

template<typename type>
inline std::vector<type> Thread_Manager::get_future_vector_reverse(std::vector<std::future<type>>& source) {
	std::vector<type> rtn(source.size());
	for (size_t i = source.size()-1; i <size_t(-1); i--) {
		source[i].wait();
		rtn[i] = source[i].get();
	}
	return rtn;
}

template<typename type, size_t size>
inline std::array<type, size> Thread_Manager::get_future_array_reverse(std::array<std::future<type>, size>& source) {
	std::array<type, size> rtn;
	for (size_t i = size-1;  i < size_t(-1); i--) {
		source[i].wait();
		rtn[i] = source[i].get();
	}
	return rtn;
}

template<typename type>
inline std::vector<type> Thread_Manager::get_future_vector(std::vector<std::future<type>>& source) {
	std::vector<type> rtn(source.size());
	for (int i = 0; i <source.size(); i++) {
		source[i].wait();
		rtn[i] = source[i].get();
	}
	return rtn;
}

template<typename type, size_t size>
inline std::array<type, size> Thread_Manager::get_future_array(std::array<std::future<type>, size>& source) {
	std::array<type, size> rtn;
	for (int i = 0; i <size; i++) {
		source[i].wait();
		rtn[i] = source[i].get();
	}
	return rtn;
}

