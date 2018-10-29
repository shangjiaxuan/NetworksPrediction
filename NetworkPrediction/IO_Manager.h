#pragma once

#include "Header.h"

#include "network_data.h"

class IO_Manager {
public:
	static network_data read_file(const std::string& file);
	static void write_sorted(const std::string& file, const network_data& data);
};
