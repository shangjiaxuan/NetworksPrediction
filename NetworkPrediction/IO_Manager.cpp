#include "IO_Manager.h"

using namespace std;

data_sets IO_Manager::read_file(const std::string& file) {
	std::ifstream ifs;
	ifs.open(file);
	if(!ifs) {
		cout << "Failed to read from input file!" << endl;
		return data_sets{};
	}
	ifs.seekg(0, ios::end);
	const uint64_t size = ifs.tellg();
	std::string str;
	str.resize(size);
	ifs.seekg(0);
	ifs.read(&str[0], size);
	ifs.close();
	network_data rtn;
	istringstream iss{str};
	rtn.max_index = 0;
	while(iss.good()) {
		int author = -1, viewer = -1, time = -1;
		if(iss >> author >> viewer >> time) {
			if(author > rtn.max_index) rtn.max_index = author;
			if(viewer > rtn.max_index) rtn.max_index = viewer;
		}
	}
	iss.clear();
	iss.seekg(0);
	bool* num_exists = new bool[rtn.max_index + 1]();
	while(iss.good()) {
		int author = -1, viewer = -1, time = -1;
		if(iss >> author >> viewer >> time) {
			num_exists[author] = true;
			num_exists[viewer] = true;
		}
	}
	iss.clear();
	iss.seekg(0);
	rtn.index = new unsigned[rtn.max_index + 1];
	rtn.num_of_people = 0;
	for(int i = 0; i <= rtn.max_index; i++) {
		if(num_exists[i]) {
			rtn.index[i] = rtn.num_of_people;
			rtn.num_of_people++;
		} else {
			rtn.index[i] = -1;
		}
	}
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people];
	rtn.people = new int[rtn.num_of_people];
	rtn.num_of_people = 0;
	for(int i = 0; i <= rtn.max_index; i++) {
		if(num_exists[i]) {
			rtn.people[rtn.num_of_people] = i;
			rtn.num_of_people++;
		}
	}
	delete[] num_exists;
	num_exists = nullptr;
	while(iss.good()) {
		int author, viewer, time;
		if(iss >> author >> viewer >> time) {
			rtn[rtn.index[author]][rtn.index[viewer]].num++;
			rtn[rtn.index[author]][rtn.index[viewer]].sum += time;
		}
	}
	iss.clear();
	iss.seekg(0);
	for(unsigned i = 0; i < rtn.num_of_people; i++) {
		for(unsigned j = 0; j < rtn.num_of_people; j++) {
			if(rtn[i][j].num) {
				rtn[i][j].data = new unsigned[rtn[i][j].num];
				rtn[i][j].num = 0;
			}
		}
	}
	while(iss.good()) {
		int author, viewer, time;
		if(iss >> author >> viewer >> time) {
			rtn[rtn.index[author]][rtn.index[viewer]].data[rtn[rtn.index[author]][rtn.index[viewer]].num] = time;
			rtn[rtn.index[author]][rtn.index[viewer]].num++;
		}
	}
	for(unsigned i = 0; i < rtn.num_of_people; i++) {
		for(unsigned j = 0; j < rtn.num_of_people; j++) {
			if(rtn[i][j].num) {
				sort(rtn[i][j].data, rtn[i][j].data + rtn[i][j].num);
				rtn.num_of_directional_edge++;
				if(rtn[j][i].num) rtn.num_of_non_directional--;
			}
		}
	}
	rtn.num_of_non_directional >>= 1;
	rtn.num_of_non_directional += rtn.num_of_directional_edge;
	const data_sets& val = Algorithms::separate_sets(rtn);
	network_data::destroy(rtn);
	return val;
}

void IO_Manager::write_sorted(ostream& ofs, const network_data& data) {
	for(unsigned i = 0; i < data.num_of_people; i++) {
		for(unsigned j = 0; j < data.num_of_people; j++) {
			if(data[i][j].num) {
				ofs << data.people[i] << '\t' << data.people[j] << '\n';
				ofs << data[i][j].num << '\t' << data[i][j].sum << '\n';
				for(unsigned k = 0; k < data[i][j].num; k++) {
					ofs << data[i][j].data[k] << '\t';
				}
				ofs << "\n\n";
			}
		}
	}
}
