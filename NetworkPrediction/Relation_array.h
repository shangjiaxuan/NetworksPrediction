#pragma once
#include "network_data.h"

struct item {
	int s_person = -1;
	int d_person = -1;
	double weight;
	list contact_list;
};

struct data_sets {
	network_data* pdata = nullptr;
	unsigned num = 0;
};

struct sorted_items {
	unsigned num = 0;
	item* data = nullptr;
};

struct set_of_sorted {
	sorted_items* data = nullptr;
	unsigned num = 0;
};
