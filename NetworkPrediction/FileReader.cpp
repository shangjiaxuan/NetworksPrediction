#include "FileReader.h"
#include <algorithm>

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
	rtn.index = new int[max + 1];
	rtn.num_of_people = 0;
	for(int i = 0; i <= max; i++) {
		if(num_exists[i]) {
			rtn.index[i] = rtn.num_of_people;
			rtn.num_of_people++;
		}
		else {
			rtn.index[i] = -1;
		}
	}
	rtn.map = new list[rtn.num_of_people * rtn.num_of_people];
	rtn.people = new int[rtn.num_of_people];
	rtn.num_of_people = 0;
	for(int i=0; i<=max; i++) {
		if (num_exists[i]) {
			rtn.people[rtn.num_of_people] = i;
			rtn.num_of_people++;
		}
	}
	while(ifs.good()) {
		int author, viewer, time;
		ifs >> author >> viewer >> time;
		rtn[rtn.index[author]][rtn.index[viewer]].num++;
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
		rtn[rtn.index[author]][rtn.index[viewer]].data[rtn[rtn.index[author]][rtn.index[viewer]].num] = time;
		rtn[rtn.index[author]][rtn.index[viewer]].num++;
		ifs.peek();
	}
	ifs.close();
	for (int i = 0; i < rtn.num_of_people; i++) {
		for (int j = 0; j < rtn.num_of_people; j++) {
			if (rtn[i][j].num>1) {
				sort(rtn[i][j].data, rtn[i][j].data+rtn[i][j].num);
			}
		}
	}
	return rtn;
}

void FileReader::write_sorted(const std::string& file, const network_data& data) {
	ofstream ofs;
	ofs.open(file);
	if (!ofs) {
		cout << "Failed to read from output file!" << endl;
		return;
	}
	for(int i=0; i<data.num_of_people;i++) {
		for (int j = 0; j < data.num_of_people; j++) {
			if(data[i][j].num) {
				ofs << data.people[i] << '\t' << data.people[j] << '\n';
				for (int k = 0; k < data[i][j].num; k++) {
					ofs << data[i][j].data[k]<<'\t';
				}
				ofs << "\n\n";
			}
		}
	}
}



