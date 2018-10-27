#pragma once

#include "Header.h"

struct list {
	int num = 0;
	int* data = nullptr;
};

struct network_data {
	list* map = nullptr;
	int num_of_people = 0;
	int* people_list = nullptr;
	list* operator[](int rol) const {
		return (map + (rol * num_of_people));
	}
};

class FileReader {
public:
	static network_data read_file(const std::string& file);
};

