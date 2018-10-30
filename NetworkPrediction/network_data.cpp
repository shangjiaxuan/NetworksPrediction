#include "network_data.h"

list::~list() {
	destroy(*this);
}

list::list(const list& source) {
	copy(source, *this);
}

list::list(list&& source) noexcept {
	move(source, *this);
}

list& list::operator=(const list& source) {
	copy(source, *this);
	return *this;
}

list& list::operator=(list&& source) noexcept {
	move(source, *this);
	return *this;
}

void list::destroy(list& source) {
	delete[] source.data;
	source.data = nullptr;
}

void list::copy(const list& source, list& dest) {
	dest.sum = source.sum;
	dest.num = source.num;
	if(dest.num) {
		dest.data = new unsigned[dest.num];
		for(unsigned i = 0; i < dest.num; i++) {
			dest.data[i] = source.data[i];
		}
	}
}

void list::move(list& source, list& dest) noexcept {
	dest.sum = source.sum;
	dest.num = source.num;
	dest.data = source.data;
	source.data = nullptr;
}

network_data::~network_data() {
	destroy(*this);
}

network_data::network_data(const network_data& source) {
	copy(source, *this);
}

network_data::network_data(network_data&& source) noexcept {
	move(source, *this);
}

network_data& network_data::operator=(const network_data& source) {
	copy(source, *this);
	return *this;
}

network_data& network_data::operator=(network_data&& source) noexcept {
	move(source, *this);
	return *this;
}

void network_data::destroy(network_data& source) {
	delete[] source.index;
	source.index = nullptr;
	delete[] source.people;
	source.people = nullptr;
	delete[] source.map;
	source.map = nullptr;
}

void network_data::copy(const network_data& source, network_data& dest) {
	dest.num_of_directional_edge = source.num_of_directional_edge;
	dest.num_of_non_directional = source.num_of_non_directional;
	dest.num_of_records = source.num_of_records;
	dest.sum_of_records = source.sum_of_records;
	dest.max_index = source.max_index;
	dest.index = new unsigned[dest.max_index];
	for(int i = 0; i < dest.max_index; i++) {
		dest.index[i] = source.index[i];
	}
	dest.num_of_people = source.num_of_people;
	dest.people = new int[dest.num_of_people];
	for(unsigned i = 0; i < dest.num_of_people; i++) {
		dest.people[i] = source.people[i];
	}
	const int map_size = dest.num_of_people * dest.num_of_people;
	dest.map = new list[map_size];
	for(int i = 0; i < map_size; i++) {
		dest.map[i] = source.map[i];
	}
}

void network_data::move(network_data& source, network_data& dest) noexcept {
	dest.num_of_directional_edge = source.num_of_directional_edge;
	dest.num_of_non_directional = source.num_of_non_directional;
	dest.num_of_people = source.num_of_people;
	dest.max_index = source.max_index;
	dest.num_of_records = source.num_of_records;
	dest.sum_of_records = source.sum_of_records;
	dest.people = source.people;
	source.people = nullptr;
	dest.index = source.index;
	source.index = nullptr;
	dest.map = source.map;
	source.map = nullptr;
}
