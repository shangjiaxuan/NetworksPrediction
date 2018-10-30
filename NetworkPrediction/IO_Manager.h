#pragma once

#include "Algorithms.h"

class IO_Manager {
public:
	static data_sets read_file(const std::string& file);
	static void write_sorted(std::ostream& ofs, const network_data& data);
};
