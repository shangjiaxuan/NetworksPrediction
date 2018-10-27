#include "FileReader.h"

using namespace std;

network_data FileReader::read_file(const std::string& file) {
	std::ifstream ifs;
	ifs.open(file);
	if(!ifs) {
		cout << "Failed to read from input file!" << endl;
		return network_data{};
	}
	int temp = -1;
	int max = 0;
	while(ifs.good()) {
		ifs >> temp;
		if(temp > max)max = temp;
		ifs >> temp;
		if(temp > max)max = temp;
		ifs >> temp;
		ifs.peek();
	}
	ifs.clear();
	ifs.seekg(0);
	bool* num_exists = new bool[max + 1]();
	while(ifs.good()) {
		ifs >> temp;
		num_exists[temp] = true;
		ifs >> temp;
		num_exists[temp] = true;
		ifs >> temp;
		ifs.peek();
	}
	ifs.clear();
	ifs.seekg(0);
	network_data rtn;
	rtn.person_index = new int[max + 1];
	rtn.num_of_people = 0;
	for(int i = 0; i < max + 1; i++) {
		if(num_exists[i]) {
			rtn.person_index[i] = rtn.num_of_people;
			rtn.num_of_people++;
		}
	}
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people];
	while(ifs.good()) {
		int author, viewer, time;
		ifs >> author >> viewer >> time;
		rtn[rtn.person_index[author]][rtn.person_index[viewer]].num++;
		ifs.peek();
	}
	ifs.clear();
	ifs.seekg(0);
	for(int i = 0; i < rtn.num_of_people; i++) {
		for(int j = 0; j < rtn.num_of_people; j++) {
			if(rtn[i][j].num) {
				rtn[i][j].data = new int[rtn[i][j].num];
				rtn[i][j].num = 0;
			}
		}
	}
	while(ifs.good()) {
		int author, viewer, time;
		ifs >> author >> viewer >> time;
		rtn[rtn.person_index[author]][rtn.person_index[viewer]].data[rtn[rtn.person_index[author]][rtn.person_index[
			viewer]].num] = time;
		rtn[rtn.person_index[author]][rtn.person_index[viewer]].num++;
		ifs.peek();
	}
	ifs.close();
	return rtn;
}
