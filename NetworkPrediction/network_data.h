#pragma once

#include "Header.h"

struct list {
	unsigned num = 0;
	uint64_t sum = 0;
	unsigned* data = nullptr;

	//reserved field for use in network data as network property
	//dynamically resembles a weight for this node
//	double weight = 0;

	list() = default;
	~list();
	list(const list& source);
	list(list&& source) noexcept;
	list& operator=(const list& source);
	list& operator=(list&& source) noexcept;
	static void move(list& source, list& dest) noexcept;
	static void copy(const list& source, list& dest);
	static void destroy(list& source);
};

struct network_data {
	list* map = nullptr;

	unsigned num_of_people = 0;
	int max_index = 0;
	unsigned num_of_directional_edge = 0;
	unsigned num_of_non_directional = 0;
	unsigned num_of_records = 0;
	uint64_t sum_of_records = 0;
	//array of the index within calculation (convert input index to true index)
	unsigned* index = nullptr;
	//array of the index the outside world knows
	int* people = nullptr;

	list* operator[](int rol) const {
		return (map + (rol * num_of_people));
	}

	network_data() = default;
	~network_data();
	network_data(const network_data& source);
	network_data(network_data&& source) noexcept;
	network_data& operator=(const network_data& source);
	network_data& operator=(network_data&& source) noexcept;
	static void move(network_data& source, network_data& dest) noexcept;
	static void copy(const network_data& source, network_data& dest);
	static void destroy(network_data& source);
};

