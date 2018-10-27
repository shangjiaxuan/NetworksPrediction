#pragma once

#include "Header.h"

struct list {
	int num = 0;
	int* data = nullptr;
};

struct network_data {
	list* map = nullptr;
	int num_of_people = 0;
	//array of the index within calculation (convert input index to true index)
	int* index = nullptr;
	//array of the index the outside world knows
	int* people = nullptr;
	list* operator[](int rol) const {
		return (map + (rol * num_of_people));
	}
};

class FileReader {
public:
	static network_data read_file(const std::string& file);
	static void write_sorted(const std::string& file, const network_data& data);
};
