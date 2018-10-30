#include "Algorithms.h"
#include "../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.17134.0/ucrt/stdlib.h"

using namespace std;

sorted_items Algorithms::func1(const network_data& data) {
	return sorted_items();
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
	return rtn;
}

