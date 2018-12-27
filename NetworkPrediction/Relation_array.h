#pragma once
#include "network_data.h"

struct item {
	int s_person = -1;
	int d_person = -1;
	double weight;
	::list* contact_list;
};

