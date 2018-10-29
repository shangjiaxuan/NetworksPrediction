#pragma once

#include "network_data.h"

struct item {
	int s_person;
	int d_person;
	int num;
	int sum;
};

struct data_sets {
	network_data* pdata = nullptr;
	int num = 0;
};

class Algorithms {
public:
	static data_sets separate_sets(network_data& source);
private:
	static network_data find_one_subset(int* people, network_data& source, int group);
	static void find_one_subset_loop(bool* cur_group, int* found, network_data& source, int cur, int start, int group);
};

