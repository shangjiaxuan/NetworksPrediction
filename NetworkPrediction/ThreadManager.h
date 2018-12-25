/*
*	Header for managing multiple threads from one main thread.
*/

#pragma once

#include <vector>
#include <array>
#include <thread>
#include <future>

class Thread_Manager {
public:
	static unsigned workers;
	static unsigned thread_used;
	//functions for multithreaded same calculation of a given array of input to an array of outputs
	template<typename input, typename output>
	static std::vector<output> vector_thread(std::vector<input>& source, output(*const function)(input&));
	template<typename input, typename output>
	static std::vector<output> vector_thread_copy(std::vector<input>& source, output(*const function)(input));
	template<typename input, typename output>
	static std::vector<std::future<output>> vector_async_copy(std::vector<input>& source, output(*const function)(input));
	template<typename input, typename output>
	static std::vector<std::future<output>> vector_async(std::vector<input>& source, output(*const function)(input&));
	template<typename input, typename output, size_t size>
	static std::array<std::future<output>, size> array_async_copy(std::array<input, size>& source, output(*const function)( input ));
	template<typename input, typename output, size_t size>
	static std::array<std::future<output>, size> array_async(std::array<input, size>& source, output(*const function)( input& ));
	template<typename type>
	static std::vector<type> get_future_vector(std::vector<std::future<type>>& source);
	template<typename type, size_t size>
	static std::array<type, size> get_future_array(std::array<std::future<type>, size>& source);
	template<typename type>
	static std::vector<type> get_future_vector_reverse(std::vector<std::future<type>>& source);
	template<typename type, size_t size>
	static std::array<type, size> get_future_array_reverse(std::array<std::future<type>, size>& source);
private:
	template<typename input, typename output>
	static void launch_function(bool* lock, output(*const function)(input&), input* data, output* out);
	template<typename input, typename output>
	static void launch_function_copy(bool* lock, output(*const function)(input), input* data, output* out);
};

#include "ThreadManager.ipp"

