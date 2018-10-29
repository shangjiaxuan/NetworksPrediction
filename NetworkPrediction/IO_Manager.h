#pragma once

#include "Header.h"

#include "network_data.h"
#include "Algorithms.h"

class IO_Manager {
public:
	static data_sets read_file(const std::string& file);
	static void write_sorted(const std::string& file, const network_data& data);
};
