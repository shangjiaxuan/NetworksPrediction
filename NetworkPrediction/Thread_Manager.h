#pragma once

#include "Relation_array.h"

typedef sorted_items (*sort_rel_func)(const network_data&);

class Thread_Manager {
public:
	static set_of_sorted work(const data_sets& source, sort_rel_func function);
};
