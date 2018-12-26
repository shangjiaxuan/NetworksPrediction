#pragma once

#include "Algorithms.h"

namespace IO_Manager {

	std::vector<network_data> read_file(const std::string& file);
	template<typename data_type>
	void write_sorted_data(std::string extension, std::vector<data_type> data, void(*writer_func)(std::ostream&, size_t, const data_type&));



	template<typename data_type>
	void write_sorted_data(std::string extension, std::vector<data_type> data, void(*writer_func)(std::ostream&, size_t, const data_type&)) {
		size_t size = data.size();
		for (unsigned i = 0; i < size; i++) {
			std::ostringstream oss;
			oss << i;
			std::ofstream ofs;
			ofs.open(oss.str() + extension);
#ifdef DEBUG
			if (!ofs) {
				std::cout << "Cannot open file for output!" << std::endl;
			}
			else {
#endif
				writer_func(ofs, i, data[i]);
#ifdef DEBUG
			}
#endif
			ofs.close();
		}
	}

	void write_network(std::ostream& ost, size_t index, const network_data& data);
}