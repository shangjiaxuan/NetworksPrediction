#include "Algorithms.h"

using namespace std;

sorted_items Algorithms::func1(const network_data& data) {
	return sorted_items();
}

bool compare_cl_coeff(const clustering& val1, const clustering& val2) {
	if (isnan(val1.cl_coeff) && isnan(val2.cl_coeff)) return val1.person < val2.person;
	if (isnan(val1.cl_coeff)) return false;
	if (isnan(val2.cl_coeff)) return true;
	if (val1.cl_coeff == val2.cl_coeff) return val1.person < val2.person;
	return(val1.cl_coeff < val2.cl_coeff);
}

counted_array<clustering> Algorithms::find_clustering_coeff(const network_data & data) {
	counted_array<clustering> rtn;
	rtn.num = data.num_of_people;
	rtn.data = new clustering[data.num_of_people];
	for(unsigned i=0; i<data.num_of_people; i++) {
		int neighbor_count = 0;
		int neighbor_edge_count = 0;
		for(unsigned j=0; j<data.num_of_people; j++) {
			if (data[i][j].num || data[j][i].num) {
				neighbor_count++;
				for(unsigned k=j+1; k<data.num_of_people; k++) {
					if((data[i][k].num||data[k][i].num)&&(data[j][k].num||data[k][j].num)) {
						neighbor_edge_count++;
					}
				}
			}
		}
		rtn.data[i].cl_coeff = double(2 * neighbor_edge_count) / (neighbor_count*(neighbor_count - 1));
		rtn.data[i].person = data.people[i];
	}
	sort(rtn.data, rtn.data + rtn.num, compare_cl_coeff);
	return rtn;
}

