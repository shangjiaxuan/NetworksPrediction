#include "IO_Manager.h"

namespace IO_Manager {
	std::vector<network_data> read_file(const std::string& file) {
		std::ifstream ifs;
		ifs.open(file);
		if (!ifs) {
			std::cout << "Failed to read from input file!" << std::endl;
			return std::vector<network_data>();
		}
		ifs.seekg(0, std::ios::end);
		const uint64_t size = ifs.tellg();
		std::string str;
		str.resize(size);
		ifs.seekg(0);
		ifs.read(&str[0], size);
		ifs.close();
		network_data rtn{};
		std::istringstream iss{ str };
		rtn.max_index = 0;
		while (iss.good()) {
			int author = -1, viewer = -1, time = -1;
			if (iss >> author >> viewer >> time) {
				if (author > rtn.max_index) rtn.max_index = author;
				if (viewer > rtn.max_index) rtn.max_index = viewer;
			}
		}
		iss.clear();
		iss.seekg(0);
		//scope for num_exists
		{
			std::vector<bool> num_exists(rtn.max_index + 1);
			while (iss.good()) {
				int author = -1, viewer = -1, time = -1;
				if (iss >> author >> viewer >> time) {
					num_exists[author] = true;
					num_exists[viewer] = true;
				}
			}
			iss.clear();
			iss.seekg(0);
			rtn.index = new unsigned[rtn.max_index + 1];
			rtn.num_of_people = 0;
			for (int i = 0; i <= rtn.max_index; i++) {
				if (num_exists[i]) {
					rtn.index[i] = rtn.num_of_people;
					rtn.num_of_people++;
				}
				else {
					rtn.index[i] = -1;
				}
			}
			try {
				rtn.map = new list[rtn.num_of_people * rtn.num_of_people];
				rtn.people = new int[rtn.num_of_people];
			}
			catch (std::bad_alloc& ba) {
				std::cerr << ba.what() << std::endl;
				network_data::destroy(rtn);
				throw std::runtime_error("Error allocating whole matrix for reading all data.");
			}
			rtn.num_of_people = 0;
			for (int i = 0; i <= rtn.max_index; i++) {
				if (num_exists[i]) {
					rtn.people[rtn.num_of_people] = i;
					rtn.num_of_people++;
				}
			}
		}
		while (iss.good()) {
			int author, viewer, time;
			if (iss >> author >> viewer >> time) {
				rtn[rtn.index[author]][rtn.index[viewer]].num++;
				rtn[rtn.index[author]][rtn.index[viewer]].sum += time;
			}
		}
		iss.clear();
		iss.seekg(0);
		try {
			for (unsigned i = 0; i < rtn.num_of_people; i++) {
				for (unsigned j = 0; j < rtn.num_of_people; j++) {
					if (rtn[i][j].num) {
						rtn[i][j].data = new unsigned[rtn[i][j].num];
						rtn[i][j].num = 0;
					}
				}
			}
		}
		catch (std::bad_alloc& ba) {
			std::cerr << ba.what() << std::endl;
			network_data::destroy(rtn);
			throw std::runtime_error("Error allocating while reading each edge.");
		}
		while (iss.good()) {
			int author, viewer, time;
			if (iss >> author >> viewer >> time) {
				rtn[rtn.index[author]][rtn.index[viewer]].data[rtn[rtn.index[author]][rtn.index[viewer]].num] = time;
				rtn[rtn.index[author]][rtn.index[viewer]].num++;
			}
		}
		for (unsigned i = 0; i < rtn.num_of_people; i++) {
			for (unsigned j = 0; j < rtn.num_of_people; j++) {
				if (rtn[i][j].num) {
					std::sort(rtn[i][j].data, rtn[i][j].data + rtn[i][j].num);
					rtn.num_of_directional_edge++;
					if (rtn[j][i].num) rtn.num_of_non_directional--;
				}
			}
		}
		rtn.num_of_non_directional >>= 1;
		rtn.num_of_non_directional += rtn.num_of_directional_edge;
		const std::vector<network_data>& val = Algorithms::separate_sets(rtn);
		network_data::destroy(rtn);
		return val;
	}
}